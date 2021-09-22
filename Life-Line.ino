/* twitter @ Pradeep99374546
   Complete project details at hacester.io/
*/

#include <M5Core2.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <time.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <Adafruit_NeoPixel.h>
#include "webpage.h"
#include "pox.h"
#include "WiFi.h"
#include "secrets.h"
#include "gps.h"
#include "webserial.h"

#define PIN            25
#define NUMPIXELS      20
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
#define BUFFER_LEN  256
#define BUFFER_LEN_TOPIC  256
byte mac[6];
char mac_Id[18];
int count = 1;
char topic[BUFFER_LEN_TOPIC];
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 150; // delay for half a second
unsigned long lastMillis = 0;
time_t now;
time_t nowish = 1510592825;
int RawValue = 0;
double Voltage = 0;
double tempC = 0;
double Sum;

//it wil set the static ip address
IPAddress local_IP(192, 168, 43, 200);
//it wil set the gateway address
IPAddress gateway(192, 168, 43, 1);
// Following three settings are optional
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

void connectAWS()
{

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);


  Serial.print("Connecting to AWS IOT");


  pixels.setPixelColor(0, pixels.Color(0, 0, 50)); // Moderately bright green color.
  pixels.setPixelColor(1, pixels.Color(0, 0, 50)); // Moderately bright green color.
  pixels.setPixelColor(2, pixels.Color(0, 0, 50)); // Moderately bright green color.
  pixels.setPixelColor(3, pixels.Color(0, 0, 50)); // Moderately bright green color.
  pixels.setPixelColor(4, pixels.Color(0, 0, 50)); // Moderately bright green color.
  pixels.setPixelColor(5, pixels.Color(0, 0, 50)); // Moderately bright green color.
  pixels.setPixelColor(6, pixels.Color(0, 0, 50)); // Moderately bright green color.
  pixels.setPixelColor(7, pixels.Color(0, 0, 50)); // Moderately bright green color.
  pixels.setPixelColor(8, pixels.Color(0, 0, 50)); // Moderately bright green color.
  pixels.setPixelColor(9, pixels.Color(0, 0, 50)); // Moderately bright green color.

  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(2000);

  while (!client.connect(THINGNAME)) {
    Serial.println(".");
    delay(100);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(topic);

  Serial.println("AWS IoT Connected!");
  M5.Lcd.setTextSize(2);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(200, "text/html", webpageCode);
  });

    server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(401);
  });

  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", logout_html);
  });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  Serial.print("server IP Address: ");
  Serial.print(WiFi.localIP());
  server.begin();
  Serial.println(" ");

  pixels.setPixelColor(0, pixels.Color(0, 50, 0)); // Moderately bright green color.
  pixels.setPixelColor(1, pixels.Color(0, 50, 0)); // Moderately bright green color.
  pixels.setPixelColor(2, pixels.Color(0, 50, 0)); // Moderately bright green color.
  pixels.setPixelColor(3, pixels.Color(0, 50, 0)); // Moderately bright green color.
  pixels.setPixelColor(4, pixels.Color(0, 50, 0)); // Moderately bright green color.
  pixels.setPixelColor(5, pixels.Color(0, 50, 0)); // Moderately bright green color.
  pixels.setPixelColor(6, pixels.Color(0, 50, 0)); // Moderately bright green color.
  pixels.setPixelColor(7, pixels.Color(0, 50, 0)); // Moderately bright green color.
  pixels.setPixelColor(8, pixels.Color(0, 50, 0)); // Moderately bright green color.
  pixels.setPixelColor(9, pixels.Color(0, 50, 0)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.

}


void publishMessage()
{
  Sum = 0;
  for (int k = 1; k <= 100; k++) {
    RawValue = analogRead(36);
    Sum += RawValue;
  }
  Sum /= 100;
  Voltage = (Sum / 2048.0) * 3300; // in millivots
  tempC = Voltage * 0.1;

  HR_loop();
  gpsloop();


  StaticJsonDocument<500> doc;
  JsonObject root = doc.to<JsonObject>();
  JsonObject state = root["state"].to<JsonObject>();
  JsonObject reported = state["reported"].to<JsonObject>();
  reported["Heart_Rate"] = pox.getHeartRate();
  reported["bpm / SpO2"] = pox.getSpO2();
  reported["Temperature"] =  tempC ;
  JsonObject Location = reported["Location"].to<JsonObject>();
  Location["latitude"] = Lat;
  Location["longitude"] = Lng;

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);
  client.publish(topic, jsonBuffer);
  Serial.println(jsonBuffer);

  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(40, 10);
  M5.Lcd.printf("Life Line by M5Stack");
  M5.Lcd.fillRect(01, 40, 150, 190, GREEN);
  M5.Lcd.fillRect(140, 40, 170, 190, YELLOW);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.setCursor(175, 80);
  M5.Lcd.printf("%.1f", tempC);
  M5.Lcd.setCursor(30, 80);
  M5.Lcd.printf("Temp");
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setCursor(170, 150);
  M5.Lcd.printf("HR: %.1f", pox.getHeartRate());
  M5.Lcd.setCursor(170, 190);
  M5.Lcd.printf("SP02: %.1f", pox.getSpO2());
  M5.Lcd.setCursor(05, 150);
  M5.Lcd.printf("Lat: %.1f", Lat);
  M5.Lcd.setCursor(05, 190);
  M5.Lcd.printf("Lng: %.1f", Lng);


  //  delay(2000);
  // M5.Lcd.fillScreen(BLACK);

  pixels.setPixelColor(4, pixels.Color(0, 50, 0));
  pixels.setPixelColor(5, pixels.Color(0, 50, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 50));
  pixels.setPixelColor(7, pixels.Color(0, 0, 50));
  pixels.show();
  //  WebSerial.println(jsonBuffer);
  WebSerial.print("Temperature: ");
  WebSerial.println(tempC);
  WebSerial.print("Heart-Rate: ");
  WebSerial.println( pox.getHeartRate());
  WebSerial.print("SP02 %: ");
  WebSerial.println( pox.getSpO2());
  WebSerial.print("Latitude: ");
  WebSerial.println(Lat);
  WebSerial.print("Longitude: ");
  WebSerial.println(Lng);
  WebSerial.println("-----------------------------------------------------------");

}


void setup() {

  M5.begin(true, true, false, true);
  Serial.begin(115200);
  connectAWS();
  webserialsetup();
  gpssetup();
  HR_setup();
  WiFi.macAddress(mac);
  snprintf(mac_Id, sizeof(mac_Id), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println(" ");
  Serial.print("Device ID: ");
  Serial.print(mac_Id);

  M5.Lcd.fillScreen(BLACK);
}

void loop() {

  AsyncElegantOTA.loop();
  now = time(nullptr);
  if (!client.connected())
  {
    connectAWS();
  }
  else
  {
    client.loop();
    if (millis() - lastMillis > 5000)
    {
      lastMillis = millis();
      snprintf(topic, BUFFER_LEN_TOPIC, "$aws/things/""%02x:%02x:%02x:%02x:%02x:%02x""/shadow/update", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
      publishMessage();
    }
  }
  pixels.show();   // This sends the updated pixel color to the hardware.
  delay(delayval); // Delay for a period of time (in milliseconds)
}
