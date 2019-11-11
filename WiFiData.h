// file WiFiData.h created 5 Dec 2017
// contains WiFi credentials (farmerkeith specific).
// class to hold data of WiFi ssid and password for several known places
#ifndef FARMERKEITH_WIFIDATA // include guard
#define FARMERKEITH_WIFIDATA

void WiFiSearch::vectorPopulate(vector<WiFiData>& v) {
  v.clear(); // delete old entries
  v.resize(14); // no. of programmed entries
  v.push_back(WiFiData {"tanderra", "SJMQLON338"});
  v.push_back(WiFiData {"tanderra_Ext", "SJMQLON338"});
  v.push_back(WiFiData {"tanderra_Bath", "SJMQLON338"});
  v.push_back(WiFiData {"tanderra_Pump", "SJMQLON338"});
  v.push_back(WiFiData {"tanderra_test", "SJMQLON338"});
  // v.push_back(WiFiData {"bbors", "aaaaaaaaa"});
  v.push_back(WiFiData {"fallatt's Wi-Fi Network", "Bow1896#"});
  v.push_back(WiFiData {"WiFi-8614", "30004405"}); // NBN at 441
  v.push_back(WiFiData {"billion941", "t#nde44a"}); // at 441
  v.push_back(WiFiData {"billion941_Repeater", "t#nde44a"});
  v.push_back(WiFiData {"antenna941", "t#nde44a"}); // at 441
  v.push_back(WiFiData {"ant941-Repeater", "t#nde44a"});
  v.push_back(WiFiData {"OPTUS_C78797", "shadyfraus44718"}); // ?mattes
  v.push_back(WiFiData {"OptusVD_Ext", "shadyfraus44718"});  // ?mattes
  v.push_back(WiFiData {"GroundLevelA", "ECC@2018!"}); // Dr. Donald Rowe's business premises
} // end of void vectorPopulate(WiFiData& v)

#endif
// ------------ end of WiFiData ------------------
