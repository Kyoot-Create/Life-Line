#include <M5Core2.h>
#include <TinyGPS++.h>
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
HardwareSerial ss(2);
float Lat;
float Lng;

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloatlat(float val, bool valid, int len, int prec)
{
  Lat = val;
}
static void printFloatlng(float val1, bool valid, int len, int prec)
{
  Lng = val1;
}

void gpssetup()
{
  M5.begin();
  ss.begin(GPSBaud);
}

void gpsloop()
{
  M5.Lcd.setCursor(0, 100);
  M5.Lcd.setTextColor(WHITE, BLACK);

  printFloatlat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloatlng(gps.location.lng(), gps.location.isValid(), 12, 6);

  M5.Lcd.println();

  smartDelay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    M5.Lcd.println(F("No GPS data received: check wiring"));
}
