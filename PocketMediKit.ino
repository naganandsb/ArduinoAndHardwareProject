#include<SoftwareSerial.h>
#include<Wire.h>
#include<Adafruit_MLX90614.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Global variables~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

SoftwareSerial nagsBlue(6,5);
char nagsReadChar ='a';
Adafruit_MLX90614 nagstempsense = Adafruit_MLX90614();

PulseOximeter pox;
uint32_t tsLastReport = 0;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Global variables~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup() {
  nagsBlue.begin(9600);
  Serial.begin(9600);
  nagstempsense.begin();

  if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update();
  if(nagsBlue.available())
  {
    nagsReadChar = nagsBlue.read();
    switch(nagsReadChar)
    {
      case '1':
      {
        Serial.println("ECG Selected"); 
        nagsBlue.write("12");
        break;
      }
      case '2':
      {
        Serial.println("Temparature Selected"); 
        nagsBlue.write(nagstempsense.readObjectTempF());
        break;
      }
      case '3':
      {
        Serial.println("Gluecometer Selected"); 
        nagsBlue.write("90");
        break;
      }
      case '4':
      {
        Serial.println("Heart beat Selected"); 
        
        if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        nagsBlue.write(pox.getHeartRate());
        Serial.print("bpm ");
        

        tsLastReport = millis();
    }
         break;
      }
      case '5': 
      {
        Serial.println("Blood pressure Selected"); 
        nagsBlue.write("89");
         break;
      }
      case '6':
      {
        Serial.println("Oximeter selected Selected"); 
        if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        nagsBlue.write(pox.getSpO2());
        Serial.print(pox.getSpO2());
        Serial.println("%");

        tsLastReport = millis();
    }
        
        break;
      }
      
      
    }
  }
  
}
