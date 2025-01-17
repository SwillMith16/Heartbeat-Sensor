#include "HeartbeatSensor.h"

/*
NOTE: This library was developed for a finger sensor. While it is still compatible with other sensors, variable names and comments are reflective of a finger sensor being used.
*/


// input pin from sensor
#define heartPin A0

HeartbeatSensor Sensor;

bool readSensor;
int startTime;

// boundary values to determine if sensor is receiving a valid reading - chosen through trial and error, not the same on every sensor
// min: 0, max: 1024
int lowerDetectBound = 540;
int upperDetectBound = 950;

void setup() {
  Serial.begin(115200);
  Sensor.begin();

  readSensor = false;
}

void loop() {
  Sensor.value = analogRead(heartPin);
  
  // boundary values chosen through trial and error - different sensors may require different boundary values
  if (Sensor.value > lowerDetectBound && Sensor.value < upperDetectBound){
    // Sensor has been pressed. Get time of when it was first pressed
    if (!readSensor){
      startTime = millis();
      readSensor = true;
    }

    // for first 2 seconds of read, just collect data. Don't output anything.
    // when ginger first placed on sensor, large spikes occur so this allows for them to pass before outputting
    if (millis() - startTime < 2000){
      Sensor.measureData();
    }
    else{
      Sensor.measureData();
      
      // plot measured value against calculated avg value
      // view this in Arduino IDE Serial Plotter for graphical view - much easier than reading the Serial Monitor!
      Serial.print("Average:");   Serial.print(Sensor.avgValue);         Serial.print(",");
      Serial.print("Baseline:");  Serial.print(Sensor.avgBaseline);      Serial.print(",");
      Serial.print("BPM:");       Serial.print(Sensor.bpm);              Serial.print(",");
      Serial.print("UpLimit:");   Serial.print(Sensor.avgBaseline + 30); Serial.print(",");
      Serial.print("LowLimit:");  Serial.print(Sensor.avgBaseline - 30); Serial.println();
    }
  }
  else{ // No pulse on the sensor
    readSensor = 0;
  }

  delay(5);
}