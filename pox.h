#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     5000

PulseOximeter pox;
uint32_t tsLastReport = 0;

void onBeatDetected()
{
  Serial.println("Beat!");
}

void HR_setup()
{

  Serial.println("Initializing pulse oximeter.......");
  //    if (!pox.begin()) {
  //        Serial.print("FAILED");
  //        for(;;);
  //    } else {
  //        Serial.print("SUCCESS");
  //    }
  pox.begin();
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void HR_loop()
{

  pox.update();
  //        Serial.print("Heart rate:");
  //        Serial.print(pox.getHeartRate());
  //        Serial.print("bpm / SpO2:");
  //        Serial.print(pox.getSpO2());
  //        Serial.println("%");

}
