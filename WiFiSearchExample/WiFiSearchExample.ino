// WiFiSearch sketch by farmerkeith 
// created 6 December 2017 based on WindMeter project (unpublished)

// debug print control
const bool wifiDebug=1; // set to 1 to enable printing, 0 to disable   
#include "WiFiSearch.h" // tab file, to become library

// ---------------------setup()-----------------------
void setup() {
  Serial.begin(115200);
  Serial.println("\nStart of WiFiSearchExample");

  WiFiSearch.debug = wifiDebug; // set debug printing in WiFiSearch library
  WiFiData WiFiUsed = WiFiSearch.Search(); // returns WiFiData object of registered network if found
  
  Serial.println ("Results of WiFiSearch:");
  if (WiFiUsed.flag==0) Serial.println (" No recognised network");
  else {
    Serial.print ("Network recognised, SSID = ");
    Serial.print (WiFiUsed.ssid);
    Serial.print (" Password= ");
    Serial.println (WiFiUsed.password);
  }

} // end of void setup() 

// -------------------- loop------------------------
void loop() {
  
} // end of void loop() 

// end of file

