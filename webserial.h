#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
String d = "";
AsyncWebServer server(80);

void recvMsg(uint8_t *data, size_t len) {
  WebSerial.println("Received Data...");
  Serial.println("Received Data...");

  for (int i = 0; i < len; i++) {
    d += char(data[i]);
  }
  WebSerial.println(d);
  Serial.println(d);
  WebSerial.println("-----------------------------------------------------------");
}

void webserialsetup() {
  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
}
