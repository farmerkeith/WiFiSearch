// file WiFiData.h created 5 Dec 2017
// contains WiFi credentials (farmerkeith specific).
// class to hold data of WiFi ssid and password for several known places
#ifndef FARMERKEITH_WIFIDATA
#define FARMERKEITH_WIFIDATA

class WiFiData {
  public:
  WiFiData();
  WiFiData(char ssid[33], char password[33]); // constructor
  char ssid[33];
  char password[33];  
  bool flag=1;
  static byte count;
};

byte WiFiData::count=0; // initialise object counter

// constructors
WiFiData::WiFiData(char s[33], char p[33]) {
  strcpy(ssid, s);
  strcpy(password, p);
  flag = 1;
  count ++;
} 

WiFiData::WiFiData(){
  ssid[0] = 0;
  password[0] = 0;
  flag = 0;
} // end of constructors

// This is the information that needs to be customised for your local WiFi network(s)
WiFiData network[] { // instantiates an array of WiFiData objects
  (WiFiData){"yourSSID","yourPassword"},
  (WiFiData){"anotherSSID","anotherPassword"},
  (WiFiData){"as many as you like SSID","yetAnother"},
  (WiFiData){"can extend indefinitely","password"},
  // this list can be one or many sets of (WiFiData){"ssid","password"},
  // do not omit the comma at the end, after the last }
}; // end of array instantiation

#endif
// ------------ end of WiFiData ------------------

