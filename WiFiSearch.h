// file WiFiSearch.h created 5 Dec 2017 by farmerkeith, updated 7 September 2019
// looks for a WiFi signal that it recognises and logs in
// The object flag is 0 if no signal or no recognised network

#ifndef FARMERKEITH_WIFISEARCH_H
#define FARMERKEITH_WIFISEARCH_H

#include <Vector_farmerkeith.h> // 
#include <FS.h> // FileSystem library, activates SPIFFS file system
#include <ESP8266WiFi.h> // library for WiFi protocol stack
String BoolToOnOff(bool Flag) {
  if (Flag) return " ON";
  else return "OFF";
} // end of String BoolToOnOff

String BoolToOKFail(bool Flag) {
  if (Flag) return " OK";
  else return "Fail";
} // end of String BoolToOKFail

String Rjust (int val) { // right justify
  if (val >= 100) return (String)val;
  if (val >= 10) return " " + (String)val;
  else return "  " + (String)val;
}

class WiFiData { // ssid, password and a flag (indicating "found")
  public:
    WiFiData();                                 // constructor
    WiFiData(char ssid[33], char password[33]); // constructor
    WiFiData(String ssid, String password);     // constructor
    char ssid[33];
    char password[33];
    bool flag = 1;
    byte index = 255;
    String out(); // return contents of data elements for output to print
    WiFiData filein(String); // break input String into consituent data elements
};

class NetworkData { // ssid, RSSI, encryption type
  public:
    String ssid{""};
    int rssi{ -1000};
    int encryptionCode{0};
    int noAccess{0};       // count of how many times since last reception
    int PresentCount{0};   // count of times seen
    int AbsentCount{0};    // count of times not seen
    float PresentRatio{0}; // ratio of PresentCount to sum of Present and Absent
    // bool doNotUse{0};      // flag to stop use of this WiFi signal
    NetworkData();         // constructor for empty object
    NetworkData(String s, int r, int code, int na); // constructor for object with WiFi received data
    String out(); // return contents of data elements for output to print
    String fileout(); // return contents of data elements for output to file
    NetworkData filein(String); // break input String into consituent data elements
    String encryptionType (int typeCode); // translate typeCode to text
};

class WiFiSearch {
  public:
    bool SearchPing(IPAddress ping_ip); // Search and Ping
    WiFiData WiFiConnected; // data object connected WiFi network
    void WiFilog(String); // print log messages to Serial and file flog
  private:
    bool debug = 0; // 1 to enable printing of progress messages
    void scanWiFi();   // function to scan WiFi and populate a vector
    WiFiData Search(); // function that does the search
    void updateWiFiList();
    WiFiData Login();  // log in to WiFiUsed network
    void getPasswordData(); // function to get password data
    int RSSI();        // get signal strength of selected network
    File flog;          // File handle for log file
    WiFiData WiFiUsed;  // data object for used WiFi network
    WiFiData checkPing(IPAddress ping_ip); // function to ping WiFiUsed
    void readExtraFile();
    void readListFile() ;
    void vectorPopulate(vector<WiFiData>& v); // programmed list of WiFi SSID and password pairs
    String WiFiStatusCode (byte code);
    byte WiFiSelected;                    // network No. of selected network
    int counter = 0;

    // int RSSI1;                         // RSSI of current network
    byte WiFiSelected1;                   // network No. of current network
    vector<WiFiData> WiFiExtra;    // vector of <ssid,password> pairs populated from file /WiFiExtra
    vector<WiFiData> WiFiProgrammed;      // vector of <ssid,password> pairs populated from the program
    vector<String> noPing;       // vector of ssid that have not worked
    vector<NetworkData> WiFiVisible;      // vector of <ssid,rssi,encryptCode> obtained from the receiver
    vector<NetworkData> WiFiList;         // vector of <ssid,rssi,encryptCode> kept in a file
    bool ssidMatch(vector<NetworkData>& v1, int i1, vector<NetworkData>& v2, int i2);
    // function to read file (eg /WiFiExtra) and populate vector v
    // bool IsOdd (int i); // only for testing
    void trim_flog(); // copy file /WiFiRegistration (flog) removing the first 1000 lines 
} WiFiSearch; // instantiates WiFiSearch

#include "WiFiData.h" // private library for WiFi access data


WiFiData::WiFiData(String s, String p) {
  int lens = s.length(); // get length
  char bufs[lens + 2];      // define buffer
  s.toCharArray(bufs, lens + 1); // populate buffer
  int lenp = p.length();
  char bufp[lenp + 2];
  p.toCharArray(bufp, lenp + 1);
  strcpy(ssid, bufs); // copy buffer into ssid array
  strcpy(password, bufp); // copy buffer into password array
  flag = 1;
  // count ++;
} // end of WiFiData::WiFiData(String s, String p)

// constructors
WiFiData::WiFiData(char s[33], char p[33]) {
  strcpy(ssid, s);
  strcpy(password, p);
  flag = 1;
  // count ++;
}

// #include "WiFiSearch.cpp"

WiFiData::WiFiData() {
  ssid[0] = 0;
  password[0] = 0;
  flag = 0;
} // end of constructors for WiFiData

String WiFiData::out() {
  String line = (String)ssid + ", " + (String)password + ", " + (String)flag ;
  return line;
}

WiFiData WiFiData::filein(String line) { // break input String into consituent data elements
  WiFiData wd1{};
  int first = line.indexOf(',');
  //  wd1.ssid = std::c_str(line.substring(0,first));
  line.substring(0, first).toCharArray(wd1.ssid, 33);
  line = line.substring(first + 1);
  first = line.indexOf(',');
  line.substring(0, first).toCharArray(wd1.password, 33);
  line = line.substring(first + 1);
  first = line.indexOf(',');
  wd1.flag = (bool)line.substring(0, first).toInt();
  return wd1;
} // end of WiFiData filein(String line)

// constructor for object with WiFi received data
NetworkData::NetworkData(String s, int r, int code, int na) {
  ssid = s;
  rssi = r;
  encryptionCode = code;
  noAccess = na;
}

NetworkData::NetworkData() { // constructor for empty object
}

String NetworkData::encryptionType (int typeCode) {
  // Encryption type codes: 2 = TKIP(WPA), 5=WEP,4=CCMP(WPA), 7=NONE, 8=AUTO
  //    Serial.println (typeCode);
  switch (typeCode) {
    case 2: return "TKIP(WPA)"; break;
    case 4: return "CCMP(WPA)"; break;
    case 5: return "WEP"; break;
    case 7: return "NONE"; break;
    case 8: return "AUTO"; break;
    case 0: return "Not Set"; break;
    default: return String(typeCode);
  } // end of switch (typeCode)
} // end of String encryptionType (int typeCode)

String NetworkData::out() {
  String ssid1 = ssid;
  for (int i = ssid.length(); i < 14; i++) ssid1 += ' ';
  String ecode = encryptionType(encryptionCode);
  for (int i = ecode.length(); i < 9; i++) ecode += ' ';
  String line = ssid1 + " (" + (String)rssi + " dbm) Encr type="
                + ecode + " " + Rjust(noAccess)  + " " +
                Rjust(PresentCount)  + " " + Rjust(AbsentCount)  + " " +
                (String)PresentRatio; // + " " + (String)doNotUse;
  return line;
} // end of String NetworkData::out()

String NetworkData::fileout() {
  String line = ssid + ", " + (String)rssi + ", " +
                (String)encryptionCode + ", " + (String)noAccess + ", " +
                (String)PresentCount + ", " + (String)AbsentCount + ", " +
                (String)PresentRatio;
  return line;
}

NetworkData NetworkData::filein(String line) { // break input String into consituent data elements
  NetworkData nd1{};
  int first = line.indexOf(',');
  nd1.ssid = line.substring(0, first);
  line = line.substring(first + 1);
  first = line.indexOf(',');
  nd1.rssi = line.substring(0, first).toInt();
  line = line.substring(first + 1);
  first = line.indexOf(',');
  nd1.encryptionCode = line.substring(0, first).toInt();
  line = line.substring(first + 1);
  first = line.indexOf(',');
  nd1.noAccess = line.substring(0, first).toInt();
  line = line.substring(first + 1);
  first = line.indexOf(',');
  nd1.PresentCount = line.substring(0, first).toInt();
  line = line.substring(first + 1);
  first = line.indexOf(',');
  nd1.AbsentCount = line.substring(0, first).toInt();
  line = line.substring(first + 1);
  first = line.indexOf(',');
  nd1.PresentRatio = line.substring(0, first).toFloat();
  line = line.substring(first + 1);
  return nd1;
} // end of NetworkData filein(String line)

// sorting criterion for NetworkData objects using PresentRatio
bool LT_PR (NetworkData nd1, NetworkData nd2) {
  return nd1.PresentRatio > nd2.PresentRatio;
}

// sorting criterion for NetworkData objects using rssi
bool LT_rssi (NetworkData nd1, NetworkData nd2) {
  return nd1.rssi > nd2.rssi;
}

String WiFiSearch::WiFiStatusCode (byte code) {
  if (code == WL_IDLE_STATUS)         return "WL_IDLE_STATUS";    // 0
  else if (code == WL_NO_SSID_AVAIL)  return "WL_NO_SSID_AVAIL";  // 1
  else if (code == WL_CONNECTED)      return "WL_CONNECTED";      // 3
  else if (code == WL_CONNECT_FAILED) return "WL_CONNECT_FAILED"; // 4
  else if (code == WL_DISCONNECTED)   return "WL_DISCONNECTED";   // 6
  else return "WiFi code not recognised";
} //end of String WiFiStatusCode

// send ping to WiFiUsed and flag that network if no success
WiFiData WiFiSearch::checkPing(IPAddress ping_ip) {
  // Serial.print("\ncheckPing");

  String line = "\nPinging tanderra_ip,";
  if (Ping.ping  (ping_ip)) {
    line += " result=success, Response time =";
    line += Ping.averageTime();
    line += "\n";
    WiFiUsed.flag = 1;
  } else {
    line += " result = no ping with ";
    line += WiFiVisible.at(WiFiUsed.index).ssid;
    line += "\n";
    WiFiUsed.flag = 0;
  }
  WiFilog (line);
  return WiFiUsed;
} // end of void WiFiSearch::checkPing()

void WiFiSearch::WiFilog (String line) {
  if (debug) {
    Serial.print (line); flog.print(line);
  }
}

void WiFiSearch::scanWiFi() {
  int RSSIbest = -1000;
  int networkCount = WiFi.scanNetworks();
  String line = "\nWiFiSearch: Seeing "
                + (String)networkCount + " network";
  line += (networkCount != 1) ? "s" : " ";
  WiFilog (line);
  WiFiVisible.clear();
  WiFiVisible.resize(networkCount);
  for (int i = 0; i < networkCount; i++) { // for each WiFi signal
    WiFiVisible.push_back(NetworkData (WiFi.SSID(i),
                                       WiFi.RSSI(i), WiFi.encryptionType(i), 0));
  }  // end of for (int i = 0; i < networkCount; i++)
  // sort WiFiVisible vector using RSSI
  std::sort(WiFiVisible.begin(), WiFiVisible.end(), LT_rssi);
  for (int i = 0; i < WiFiVisible.size(); i++) {
    WiFilog("\n" + (String)(i) + " " + WiFiVisible.at(i).out());
  }
  // }
} // end of scanWiFi

bool WiFiSearch::ssidMatch(vector<NetworkData>& v1, int i1, vector<NetworkData>& v2, int i2) {
  return (v1.at(i1).ssid == v2.at(i2).ssid);
}

void WiFiSearch::readListFile() {
  File flist = SPIFFS.open("/WiFiList", "r");  // opens /WiFiList file for reading
  int sz = flist.size();
  int poz = 0;
  WiFilog("\nReading file /WiFiList and populating vector WiFiList");
  WiFiList.resize(WiFiList.size()); // resize to existing No. of entries
  WiFiList.clear(); // delete old entries
  while (poz < sz) {
    String line = flist.readStringUntil('\n');
    NetworkData nd1 = nd1.filein(line);
    poz = flist.position(); // position in file
    WiFiList.push_back(nd1);
    // if (WiFiList.size()<wifiListPrint) WiFilog("\n " + nd1.out());
  }  // do again if not at end of file
  flist.close();
  WiFilog("\nNo. of WiFiList entries is " + (String)WiFiList.size());
} // end of void WiFiSearch::readListFile()

void WiFiSearch::updateWiFiList() {
  if (wifiListPrint) WiFilog ("\nupdateWiFiList");
  readListFile();

  /*  if (wifiListPrint) {
    WiFilog("\nList of currently received WiFi signals");
    for (int i = 0; i<WiFiVisible.size() && i<wifiListPrint ; i++) {
      WiFilog("\n" + WiFiVisible.at(i).out());
    }
  } */

  WiFilog ("\nNo. of currently received WiFi signals=" + (String)WiFiVisible.size());
  // update vector WiFiList with the currently received WiFi signals
  for (int i = 0; i < WiFiVisible.size(); i++) { // for each currently received WiFi signal
    int j;
    for (j = 0; j < WiFiList.size(); j++) { // scan the stored signal list
      if (ssidMatch(WiFiVisible, i, WiFiList, j)) break;
    }
    if (j == WiFiList.size()) {
      WiFiList.push_back(WiFiVisible.at(i)); // signal not in list
      Serial.print("\nadding signal to list " + WiFiVisible.at(i).out());
      // + (String)(100 * i + j));
    }
    else { // signal was found in the stored signal list at position j
      WiFiList.at(j).rssi = WiFiVisible.at(i).rssi;
      WiFiList.at(j).noAccess = 0;
      // if (wifiListPrint)
      //   Serial.print("\nupdating signal in list " + WiFiList.at(j).out() + (String)(100 * i + j));
    }
  } // end of for (int i = 0; i < WiFiVisible.size(); i++)

  // update vector WiFiList for entries not seen on this cycle
  for (int j = 0; j < WiFiList.size(); j++) { // scan the stored signal list
    int i;
    for (i = 0; i < WiFiVisible.size(); i++) { // for each currently received WiFi signal
      if (ssidMatch(WiFiVisible, i, WiFiList, j)) break;
    }
    if (i == WiFiVisible.size()) { // WiFiList entry was not received this time
      ++WiFiList.at(j).noAccess;
      ++WiFiList.at(j).AbsentCount;
      WiFiList.at(j).PresentRatio = (float)WiFiList.at(j).PresentCount / (WiFiList.at(j).PresentCount + WiFiList.at(j).AbsentCount);
      // Serial.print("\nWiFiList entry updated " + WiFiList.at(j).out());
    } else { // WiFiList entry was received this time
      ++WiFiList.at(j).PresentCount;
      WiFiList.at(j).PresentRatio = (float)WiFiList.at(j).PresentCount / (WiFiList.at(j).PresentCount + WiFiList.at(j).AbsentCount);
    }
  } // end of for (int j=0; j<WiFiList.size(); j++)
  // sort WiFiList vector using PresentRatio
  std::sort(WiFiList.begin(), WiFiList.end(), LT_PR);
  if (wifiListPrint) {
    WiFilog("\n\nList of saved WiFi signals\n");
    for (int i = 0; i < WiFiList.size() && i<wifiListPrint; i++) {
      WiFilog (WiFiList.at(i).out() + "\n");
    }
  }
  WiFilog ("\nNo. of saved WiFi signals=" + (String)WiFiList.size());
  // re-write WiFiList file
  File flist = SPIFFS.open("/WiFiList", "w");  // opens /WiFiList file for writing
  for (int i = 0; i < WiFiList.size(); i++) {
    flist.println(WiFiList.at(i).fileout());
  }
  flist.close();
} // end of void WiFiSearch::updateWiFiList

void WiFiSearch::readExtraFile() {
  
  File fextra = SPIFFS.open("/WiFiExtra", "r");  // opens /WiFiExtra file for reading
  int sz = fextra.size();
  int poz = 0;
  WiFilog("\nReading file /WiFiExtra and populating vector WiFiExtra\n");
  WiFiExtra.clear(); // delete old entries
  // WiFiExtra.resize(2); // resize to expected No. of entries
  while (poz < sz) { // do until end of file
    String line = fextra.readStringUntil('\n');
    WiFiData wd1 = wd1.filein(line);
    poz = fextra.position(); // position in file
    WiFiExtra.push_back(wd1);
    WiFilog(wd1.out());
  }
  WiFilog("\nNo. of WiFiExtra entries is " + (String)WiFiExtra.size());
  fextra.close();

} // end of void readExtraFile()

void WiFiSearch::getPasswordData() {
  
  // preamble
  WiFilog ("\nStart of WiFiSearch.getPasswordData");

  readExtraFile();   // read WiFiExtra file and store it in a vector

  // load and check contents of vector of programmed WiFi ssid and password pairs
  vectorPopulate(WiFiProgrammed);
  WiFilog("\nNo. of programmed entries is " + (String)WiFiProgrammed.size());
 
} // end of void WiFiSearch::getPasswordData(File flog)

WiFiData WiFiSearch::Search() {
  // preamble
  if (wifiListPrint) WiFilog ("\nStart of WiFiSearch.Search");
  // WiFilog("\nChecking for a known WiFi signal");
  // for each entry in WiFiExtra, look for a WiFi Signal in WiFiVisible.
  for (int j = 0; j < WiFiExtra.size(); j++) { // j is the index for the file
    for (int i = 0; i < WiFiVisible.size(); i++) { // i is the index for the signal
      if (WiFiVisible.at(i).ssid == WiFiExtra.at(j).ssid) {
        // if (WiFiVisible.at(i).doNotUse == 0 && WiFiVisible.at(i).ssid == WiFiExtra.at(j).ssid) {
        // found!
        WiFiUsed = WiFiExtra.at(j);
        WiFiUsed.flag = 1;
        WiFiUsed.index = i; // index in WiFiVisible
        WiFilog("\nSignal ");
        WiFilog(WiFiUsed.ssid);
        WiFilog(" found in WiFiExtra");
        return WiFiUsed;
      }
    } // end of i loop
  }   // end of j loop
  // for each entry in WiFiProgrammed, look for a WiFi Signal in WiFiVisible.
  int j;
  for (int i = 0; i < WiFiVisible.size(); i++) { // i is the index  for the signal
    for (j = 0; j < WiFiProgrammed.size(); j++) { // j is the index for the programmed list
      if (WiFiVisible.at(i).ssid == WiFiProgrammed.at(j).ssid) {
        // if (WiFiVisible.at(i).doNotUse == 0 && WiFiVisible.at(i).ssid == WiFiProgrammed.at(j).ssid) {
        // found!
        WiFiUsed = WiFiProgrammed.at(j);
        WiFiUsed.flag = 1;
        WiFiUsed.index = i; // index in WiFiVisible
        WiFilog("\nSignal ");
        WiFilog(WiFiUsed.ssid);
        WiFilog(" found in WiFiProgrammed");
        return WiFiUsed;
      }
    } // end of j loop
  }   // end of i loop
  if (j == WiFiProgrammed.size()) { // no recognized WiFi signal
    WiFilog("\nNo recognized WiFi signal");
    return WiFiData{};
  } else {
    WiFilog("\nUnexpected program result");
    return WiFiData{};
  }
  Serial.println();
} // end of WiFiData WiFiSearch::Search()

// *************************************************
// decode encryption codes for WiFi networks

int WiFiSearch::RSSI() {
  // return signal strength of selected network
  return WiFi.RSSI(WiFiSelected);
}

WiFiData WiFiSearch::Login () {
  WiFilog ("\nLogging in to " + (String)WiFiUsed.ssid
           + " at " + (String)((float)millis() / 1000) + "s");
  unsigned long startTime = millis();
  // ------------ connect to WiFi ---------------------------
  WiFi.begin(WiFiUsed.ssid, WiFiUsed.password);
  WiFilog ("\nTrying WiFi, result code ");
  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); // Can't replace with timer because WiFi.begin is blocking (??)
    WiFilog ((String)WiFi.status() + " ");
    count++;
    if (count % 15 == 0) {
      WiFilog (" " + (String)count + "\n");
    }
    if (count >= 40) break;
  } // end of while (WiFi.status() != WL_CONNECTED)
  if (WiFi.status() == WL_CONNECTED) {
    WiFilog ("\nSuccessful connection to " + (String)WiFiUsed.ssid
             + " at " + (String)((float)millis() / 1000) + "s");
    WiFilog ("\nConnection delay " + (String)((float)(millis() - startTime) / 1000) + "s");
  } else {
    WiFiUsed.flag = 0;
    WiFilog ("\nNo connection, code is " + WiFiStatusCode(WiFi.status()));
  }
  digitalWrite(LEDpin, HIGH);   // turns the LED off
  return WiFiUsed;
} // end of WiFiData WiFiSearch::Login ()

void WiFiSearch::trim_flog() { // copy file /WiFiRegistration (flog) removing the first 1000 lines 
  String line = "\ntrimming file /WiFiRegistration"; 
  line += " at time " + (String)((float)millis()/1000);
  WiFilog (line);
  flog.close();
  SPIFFS.rename("/WiFiRegistration", "/WiFiRegistration1"); // rename the file prior to copying row by row
  File flog1 = SPIFFS.open("/WiFiRegistration1", "r");  // opens renamed file for reading
  File flog2 = SPIFFS.open("/WiFiRegistration", "w");  // opens original file for writing
  if (flog1) { // flog1 is open to read (source file)
    String s, s1;
    int counter = 0;
    while (flog1.position() < flog1.size()) 
    { s = flog1.readStringUntil('\n');
      s.trim();
      if (flog1.position() > 10000) 
        flog2.println(s); // copy into destination file
      counter ++;
    } // end of while (flog1.position() < flog1.size())
    flog1.close(); flog2.close();
    SPIFFS.remove("/WiFiRegistration1"); // delete copy file
  } else {
    Serial.print("\nFile could not be opened");
    flog2.close();
  } //  end of else part of if (flog1)
  File flog = SPIFFS.open("/WiFiRegistration", "a");
  line = "\nfile trim completed at time "; 
  line += (String)((float)millis()/1000);
  WiFilog (line);
} // end of void WiFiSearch::trim_flog()

bool WiFiSearch::SearchPing(IPAddress ping_ip) { // Search and Ping
  // plan of steps in this function
  // 0. setup section
  // 1. clear noPing list
  // 2. DO: scan WiFi - put results in WiFiVisible vector
  // 3. delete any WiFis that are in the noPing list
  // 4. find a WiFi in Extra or Program list - found or none found
  // 5. if none found, exit with no success
  // 6. if success, Login to WiFi (unless already logged in) - success or fail
  // 7. if fail, add that WiFi to noPing list and scan WiFi again
  // 8. if success, Ping using WiFi - success or fail
  // 9. if Ping fail, disconnect and add that WiFi in noPing list and scan WiFi again
  // 10. exit with result - success if Ping OK, otherwise no success
  // 0. setup section
  debug = 1;
  bool spiffsOK = SPIFFS.begin();
  flog = SPIFFS.open("/WiFiRegistration", "a");  // opens /WiFiRegistration file for append
  if (flog) { // log file is OK
    String line = "\n\nSearchPing Spiffs begin is " + BoolToOKFail(spiffsOK);
    line += " at time " + (String)((float)millis()/1000);
    WiFilog (line);
    counter ++;
    WiFilog ("\nNo. of cycles since restart=" + (String)counter);
    getPasswordData(); // populates vectors with data: WiFiExtra file + programmed
    // scan WiFi networks and save it in vector WiFiVisible
    // 1. clear noPing list
    // WiFilog ("\nnoPing.clear()");
    noPing.clear();
    // 2. DO: scan WiFi - put results in WiFiVisible vector
    bool pingOK = 0;
    do {
      // WiFilog("\nline 542");
      flog.close();
      if (flog==0) flog=SPIFFS.open("/WiFiRegistration", "a"); 
      scanWiFi();
      // 3. delete any WiFis that are in the noPing list
      for (byte i = 0; i < noPing.size(); i++) {
        for (byte j = 0; j < WiFiVisible.size(); j++) {
          if (noPing.at(i) == WiFiVisible.at(j).ssid) {
            String line = "\ndeleting " + noPing.at(i) + " from WiFiVisible";
            WiFilog (line);
            WiFiVisible.erase (WiFiVisible.begin() + j);
          }
        }
      }
      // 4. find a WiFi in Extra or Program list - found or none found
      WiFiUsed = Search(); // returns WiFiData object of recognised network if found
      if (WiFiUsed.flag == 0) { // no WiFi signal recognised
        // 5. if none found, exit with no success (?wait 5s and do again?)
        break; // exit with no success
      } else { // success
        // 6. if success, Login to WiFi (unless already logged in) - success or fail
        updateWiFiList();
        // if (WiFi.status() != WL_CONNECTED) // log in only if not connected
        if (WiFiConnected.flag == 0) // log in only if not connected
          WiFiConnected = Login();
        String line = "\nWiFi status is " + WiFiStatusCode(WiFi.status());
        if (WiFi.status() == WL_CONNECTED) {
          line += " with " + (String)WiFiConnected.ssid;
        }
        WiFilog (line);
        // 7. if fail, add that WiFi to noPing list and scan WiFi again
        if (WiFiConnected.flag == 0 ) { // login failed
          line = "\n" + (String)WiFiConnected.ssid + " added to noPing list";
          WiFilog (line);
          noPing.push_back (WiFiConnected.ssid);
          pingOK = 0;
        } else { // login success
          // 8. if success, Ping using WiFi - success or fail
          WiFiUsed = checkPing(ping_ip);
          // 9. if Ping fail, disconnect and add that WiFi in noPing list and scan WiFi again
          if (WiFiUsed.flag == 0) { // Ping fail
            line = "\n" + (String)WiFiConnected.ssid + " added to noPing list and WiFi disconnected";
            WiFilog (line);
            noPing.push_back (WiFiConnected.ssid);
            WiFi.disconnect();
          }
          pingOK = WiFiUsed.flag;
        }
        // 10. exit with result - success if Ping OK, otherwise no success
      } // end of else part of if (WiFiUsed.flag == 0)
    } while (pingOK == 0); // end of do
    // flog.close();
    long flog_size = flog.size();
    WiFilog ("log file WiFiRegistration size()=" + (String)flog_size + "\n");
    if (flog_size > 32000) trim_flog();
    return pingOK;
  } // end of if (flog)
  else {
    Serial.println("Could not open log file \"/WiFiRegistration\", cycle terminated.");
    return 0;
  }
} // end of bool WiFiSearch::SearchPing()

#endif // end of #ifndef FARMERKEITH_WIFISEARCH_H
// end of file
