// file WiFiSearch.h created 5 Dec 2017 by farmerkeith
// looks for a WiFi signal that it recognises and returns an object of type WiFiData
// The object flag is 0 if no signal or no recognised network

#ifndef FARMERKEITH_WIFISEARCH_H
#define FARMERKEITH_WIFISEARCH_H

#include "WiFiData.h" // private library for WiFi access data
#include <ESP8266WiFi.h> // library for WiFi protocol stack

class WiFiSearch {
  public:
    WiFiData Search(); // function that does the search
    int RSSI();      // get signal strength of selected network
    bool debug = 0; // 1 to enable printing of progress messages
  private:
    String encryptionType (int typeCode); // function to decode encryption code of a WiFi signal
    byte WiFiSelected;                    // network No. of selected network
} WiFiSearch; // instantiates WiFiSearch

WiFiData WiFiSearch::Search() {
  int networkCount = WiFi.scanNetworks();
  if (debug) {
    Serial.print (" WiFiSearch: Seeing ");
    Serial.print (networkCount);
    Serial.print (" network");
    if (networkCount != 1) Serial.print ('s');
    Serial.println ();
  }
  WiFiData WiFiTemp; // local object of type WiFiData, initialised empty, flag=0
  // check if any network is recognised, and if so set up for log in
  for (int i = 0; i < networkCount; i++) {
    String ssidTemp = WiFi.SSID(i);
    if (debug) {
      // Print SSID, RSSI and Encryption for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i)); // received signal strength
      Serial.print(" dBm) Encryption type= ");
      Serial.println(encryptionType(WiFi.encryptionType(i)));
      // Encryption type codes: 2 = TKIP(WPA), 5=WEP, 4=CCMP(WPA), 7=NONE, 8=AUTO
    } // end of if (debug)

    for (int j = 0; j < WiFiData::count; j++) { // check against recognised networks
      if (ssidTemp == (String)network[j].ssid) {
        WiFiTemp = network[j];
        WiFiSelected = i;
      }
    }

  } // end of ssid loop for (int i = 0; i < networkCount; i++)

  if (debug) {
    if (WiFiTemp.flag) {
      Serial.println("WiFiSearch results:");
      Serial.print (" saved SSID=");
      Serial.println (WiFiTemp.ssid);
      Serial.print (" saved password=");
      Serial.println (WiFiTemp.password);
    } else { // no network recognised
      Serial.println("No recognised network");
    } // end of if (networkFlag)
  }
  return WiFiTemp;
} // end of WiFiData WiFiSearch::Search()

// *************************************************
// decode encryption codes for WiFi networks
String WiFiSearch::encryptionType (int typeCode) {
  // Encryption type codes: 2 = TKIP(WPA), 5=WEP,4=CCMP(WPA), 7=NONE, 8=AUTO
  //    Serial.println (typeCode);
  switch (typeCode) {
    case 2: return "TKIP(WPA)"; break;
    case 4: return "CCMP(WPA)"; break;
    case 5: return "WEP"; break;
    case 7: return "NONE"; break;
    case 8: return "AUTO"; break;
    default: return String(typeCode);
  } // end of switch (typeCode)
} // end of String WiFiSearch::encryptionType (int typeCode)

int WiFiSearch::RSSI() {
  // return signal strength of selected network
  return WiFi.RSSI(WiFiSelected);
}

#endif
// end of file

