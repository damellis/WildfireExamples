/*
IMPORTANT NOTE about TembooAccount.h

TembooAccount.h contains your Temboo account information and must be included
alongside your sketch. To do so, make a new tab in Arduino, call it TembooAccount.h,
and copy this content into it. 
*/

#define TEMBOO_ACCOUNT ""  // Your Temboo account name 
#define TEMBOO_APP_KEY_NAME ""  // Your Temboo app key name
#define TEMBOO_APP_KEY ""  // Your Temboo app key

#define WIFI_SSID "MIT GUEST"
#define WIFI_PASS ""
#define WIFI_SEC  WLAN_SEC_UNSEC

#define GMAIL_USER ""
#define GMAIL_PASS ""

/* 
The same TembooAccount.h file settings can be used for all Temboo SDK sketches.  
Keeping your account information in a separate file means you can share the 
main .ino file without worrying that you forgot to delete your credentials.
*/
