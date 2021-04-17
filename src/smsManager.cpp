#include <Arduino.h>
#include <WiFi.h>

#include "secrets.h"
#include "smsMessageContent.h"
#include "smsRecipient.h"

void printSmsMessage() {
  Serial.print("SMS Message: ");
  Serial.println(getSmsMessage().c_str());
}

void printSmsRecipient() {
  Serial.print("Recipient phone number: ");
  Serial.println(getSmsNumber().c_str());
}

void printSmsInformation() {
  printSmsRecipient();
  printSmsMessage();
}

void setupWifi() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to wifi network ");
  Serial.print(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, ssidPass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void notifySmsManager(const int input) {
  switch (input) {
    case 7:
      setupWifi();
      break;
    default:
      break;
  }
}