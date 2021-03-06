#include <SPI.h>
#include <Temboo.h>
#include <Adafruit_CC3000.h>

#include "TembooAccount.h"

int redpin = 8, greenpin = 9, bluepin = 10;

Adafruit_CC3000 cc3000(21, 22, 23); // CS, IRQ, VBEN
Adafruit_CC3000_Client client;
unsigned long ip;

void setup() {
  Serial.begin(9600);

  Serial.println(F("Initializing CC3000..."));

  if (!cc3000.begin()) {
    Serial.println(F("cc3000.begin() failed."));
    while (1);
  }
  
  Serial.print(F("Connecting to ")); Serial.print(WIFI_SSID); Serial.println(F("..."));
  
  while (!cc3000.connectToAP(WIFI_SSID, WIFI_PASS, WIFI_SEC)) {
    Serial.println(F("cc3000.connectToAP() failed. retrying in 10 seconds."));
    delay(10000);
  }
  
  Serial.print(F("Getting IP addresss..."));
  while(!cc3000.checkDHCP()) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  
  /* Display the IP address DNS, Gateway, etc. */  
  while (!displayConnectionDetails()) {
    delay(1000);
  }
  
  Serial.println(F("Connected."));
}


void loop() {
  Serial.println(F("Running GetWeatherByAddress"));

  TembooChoreo GetWeatherByAddressChoreo(client);

  // Invoke the Temboo client
  GetWeatherByAddressChoreo.begin();

  // Set Temboo account credentials
  GetWeatherByAddressChoreo.setAccountName(TEMBOO_ACCOUNT);
  GetWeatherByAddressChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  GetWeatherByAddressChoreo.setAppKey(TEMBOO_APP_KEY);

  // Set Choreo inputs
  String AddressValue = "San Diego, CA";
  GetWeatherByAddressChoreo.addInput("Address", AddressValue);
  
  // Filter Choreo output
  GetWeatherByAddressChoreo.addOutputFilter("temperature", "/rss/channel/item/yweather:condition/@temp", "Response");

  // Identify the Choreo to run
  GetWeatherByAddressChoreo.setChoreo("/Library/Yahoo/Weather/GetWeatherByAddress");
  
  // Run the Choreo; when results are available, print them to serial
  GetWeatherByAddressChoreo.run();
  
  String code;
  int temp, redval, blueval;

  while (GetWeatherByAddressChoreo.available()) {
    String key = GetWeatherByAddressChoreo.readStringUntil('\x1F');
    String value = GetWeatherByAddressChoreo.readStringUntil('\x1E');
    
    key.trim(); value.trim();
    
    Serial.print(key);
    Serial.print("\t");
    Serial.print(value);
    Serial.println();
    
    if (key == "temperature") temp = value.toInt();
    if (key == "HTTP_CODE") code = value;
  }
  GetWeatherByAddressChoreo.close();
  
  Serial.print("Temperature: ");
  Serial.println(temp);
  Serial.print("HTTP code: ");
  Serial.println(code);
  
  // if successful, print the new forecast.
  if (code.equals("200")) {
    Serial.println("Success!");
    
    redval = map(temp, 45, 100, 0, 255);
    redval = constrain(redval, 0, 255);
    analogWrite(redpin, 255 - redval);
    Serial.print("Red: "); Serial.println(redval);
    
    blueval = map(temp, 0, 55, 255, 0);
    blueval = constrain(blueval, 0, 255);
    analogWrite(bluepin, 255 - blueval);
    Serial.print("Blue: "); Serial.println(blueval);

    delay(4L * 60 * 60 * 1000); // on success, wait four hours
  } else {
    delay(30L * 1000); // otherwise, wait 30 seconds
  }
}

bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}
