#include <SPI.h>
#include <Temboo.h>
#include <Adafruit_CC3000.h>

#include "TembooAccount.h"

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
  Serial.println(F("Running Choreo"));

  TembooChoreo Choreo(client);

  // Invoke the Temboo client
  Choreo.begin();

  // Set Temboo account credentials
  Choreo.setAccountName(TEMBOO_ACCOUNT);
  Choreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  Choreo.setAppKey(TEMBOO_APP_KEY);

  // Set Choreo inputs
  Choreo.addInput("From", GMAIL_ADDRESS);
  Choreo.addInput("AccessToken", GMAIL_ACCESS_TOKEN);
  String MessageBodyValue = String("A0 = ") + analogRead(A0) +
                            String(", A1 = ") + analogRead(A1) +
                            String(", A2 = ") + analogRead(A2) +
                            String(", A3 = ") + analogRead(A3) +
                            String(", A4 = ") + analogRead(A4) +
                            String(", A5 = ") + analogRead(A5);
  Choreo.addInput("MessageBody", MessageBodyValue);
  Choreo.addInput("Subject", "Analog input values.");
  Choreo.addInput("To", "mellis@media.mit.edu");

  // Identify the Choreo to run
  Choreo.setChoreo("/Library/Google/Gmailv2/Messages/SendMessage/");
  
  // Run the Choreo; when results are available, print them to serial
  Choreo.run();
  
  boolean success = false;
  
  while (Choreo.available()) {
    Serial.write(Choreo.read());
  }
  Choreo.close();
  
  delay(4L * 60 * 60 * 1000); // wait four hours
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
