/*
EXAMPLE: Plot_Heartrate.ino
AUTHOR: SwillMith16

EXPLANATION:
If the user's finger is not on the sensor, the program will stay in the INACTIVE state. This measures data but doesn't calculate BPM.
If the reading drops, the user may have their finger on the sensor so the program enters the SETTLING state. More data is measured while
the program waits to see if the data reading will settle.
If the data reading has settled, the user's finger is definitely on the sensor so the program enters the PENDING state. For several seconds
data is collected so an accurate BPM can be provided.
After the pending state, th program moves into the ACTIVE state, where the BPM is calculated and output to the user.

NOTES:
This example, and the library, was developed for a generic Photoplethysmography (PPG) heart rate sensor.
For different sensor models, you may need to adjust the values of the defined constants.
Viewing the plotted data in the Arduino IDE Serial Plotter is very helpful with this!
*/

// Includes
#include "HeartbeatSensor.h"

// Macros
#define heartPin 14       // input pin from sensor
#define pulseLedPin 4     // output pulse flash LED pin

// Class instances
HeartbeatSensor Sensor;

// Constants
const int settleTime = 2000;
const int pendingTime = 5000;
const int upperDetectBound = 1000; // 0 to 1024

// Variables
int startTimer, restTimer, previousValue, value, bpm;
HeartbeatSensor::states_e state;

// Function Declarations
void plotData();

// Function Definitions
void setup() {
  Serial.begin(115200);
  Sensor.begin(50);
  analogReadResolution(10);

  pinMode(pulseLedPin, OUTPUT);
  digitalWrite(pulseLedPin, LOW);

  state              = Sensor.INACTIVE;

  startTimer         = 0;
  restTimer          = 0;

  previousValue      = 0;
  value              = 0;

  bpm                = 0;
}

void loop() {
  Sensor.value = analogRead(heartPin);
  value = Sensor.avgValue;

  if (value > upperDetectBound) state = Sensor.INACTIVE;

  switch(state)
  {
    // Finger is not on the sensor
    case Sensor.INACTIVE:
      bpm = 0;
      if (value > upperDetectBound) {
        restTimer = millis();
      }
      else if(previousValue > upperDetectBound){
        restTimer = millis();
        state = Sensor.SETTLING;
      }
      Sensor.measureData();
      digitalWrite(pulseLedPin, LOW);
    break;
    
    // Finger may be on the sensor, need to let the readings settle
    case Sensor.SETTLING:
      if (millis() - restTimer > settleTime){
        startTimer = millis();
        state = Sensor.PENDING;
      }
      Sensor.measureData();
    break;
    
    // Finger is confirmed to be on the sensor. Gather data for calculations
    case Sensor.PENDING:
      if (millis() - startTimer > pendingTime){
        state = Sensor.ACTIVE;
      }
      Sensor.measureData();
    break;
    
    // Enough steady data to calculate BPM. Calculate and output patient BPM
    case Sensor.ACTIVE:
      Sensor.measureData();
      bpm = Sensor.getBPM();

      if (Sensor.pulseDetected)
      {
        digitalWrite(pulseLedPin, HIGH);
      }
      else digitalWrite(pulseLedPin, LOW);

      if (value > upperDetectBound){
        Sensor.resetValues();
        state = Sensor.INACTIVE;
      }
    break;
  }

  plotData();

  previousValue = value;
  delay(5);
}

void plotData(){
  // view this in Arduino IDE Serial Plotter for graphical view - much easier than reading the Serial Monitor!
  Serial.print("Average:");   Serial.print(Sensor.avgValue);         Serial.print(",");
  Serial.print("Baseline:");  Serial.print(Sensor.avgBaseline);      Serial.print(",");
  Serial.print("BPM:");       Serial.print(bpm);                     Serial.print(",");

  // These are plotted to fix the scales, otherwise the axis will constanly adjust to the data, making it hard to read
  Serial.print("UpLimit:");   Serial.print(1024);                    Serial.print(",");
  Serial.print("LowLimit:");  Serial.print(0);                       Serial.println();
}
