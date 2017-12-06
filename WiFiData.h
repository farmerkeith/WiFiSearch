// file WiFiData.h created 5 Dec 2017
// contains WiFi credentials (farmerkeith specific).
// class to hold data of WiFi ssid and password for several known places

class WiFiData {
  public:
  WiFiData();
  WiFiData(char ssid[33], char password[33], bool flag); // constructor
  char ssid[33];
  char password[33];  
  bool flag=1;
  static byte count;
};

byte WiFiData::count=0; // initialise object counter

// constructors
WiFiData::WiFiData(char s[33], char p[33], bool f) {
  strcpy(ssid, s);
  strcpy(password, p);
  flag = f;
  count ++;
} 

WiFiData::WiFiData(){
  ssid[0] = 0;
  password[0] = 0;
  flag = 0;
//  count --;
//  WiFiData {"aa","bb",0};
} // end of constructors

// This is the information that needs to be customised for your local WiFi network(s)
WiFiData network[] { // instantiates array 
  (WiFiData){"yourSSID","yourPassword",1},
  (WiFiData){"anotherSSID","anotherPassword",1},
  (WiFiData){"as many as you like SSID","yetAnother",1},
  (WiFiData){"can extend indefinitely","password",1},
  // this list can be one or many sets of (WiFiData){"ssid","password",1},
  // the last element, 1, is the flag that tells the software that 
  // this is a recognised network
  // do not omit the comma at the end, afer the last }
}; // end of array instantiation

// ------------ end of WiFiData ------------------

