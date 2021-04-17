#include <Arduino.h>
#include <WiFi.h>

#include "secrets.h"
#include "smsMessageContent.h"
#include "smsRecipient.h"
#include "twilio.hpp"


void printSmsMessage() {
  Serial.print("SMS Message: ");
  Serial.println(getSmsMessage());
}

void printSmsRecipient() {
  Serial.print("Recipient phone number: ");
  Serial.println(getSmsNumber());
}

void printSmsInformation() {
  printSmsRecipient();
  printSmsMessage();
}

void sendSms() {
  Serial.println("Preparing to send sms...");
  Twilio *twilio;
  delay(1000);

  String toNumber = "+1";
  toNumber += getSmsNumber();

  twilio = new Twilio(twilioAccountSid, twilioAuthToken);

  String response;
  bool success = twilio->send_message(toNumber, twilioFromNumber,
                                      getSmsMessage(), response);
  if (success) {
    Serial.println("Sent message successfully!");
  } else {
    Serial.println(response);
  }
}

void setupWifi() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to wifi network ");
  Serial.print(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, ssidPass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("WiFi connected - IP address: ");
  Serial.println(WiFi.localIP());
}


void notifySmsManager(const int input) {
  switch (input) {
    case 7:
      setupWifi();
      break;
    case 4:
      sendSms();
      break;
    default:
      break;
  }
}