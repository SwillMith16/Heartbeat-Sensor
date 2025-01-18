#ifndef HEARTBEAT_SENSOR_H
#define HEARTBEAT_SENSOR_H

#include <Arduino.h>

const int RAW_ARRAY_LENGTH = 30;
const int AVG_ARRAY_LENGTH = 200;
const int PERIOD_ARRAY_LENGTH = 10;

class HeartbeatSensor
{
  private:
    int rawIndex, avgIndex, periodIndex, pulseTime, lastPulseTime, pulsePeriod, bpm;

  public:
    // function declarations
    double getArrayAvg(int array[], int arrayLength);
    double getArrayAvg(double array[], int arrayLength);
    int getBPM();
    void begin(double limit), measureData(), resetValues();

    // variables
    double avgValue, avgBaseline, avgPulsePeriod, detectionLimit;
    int value;
    bool pulseDetected;

    // containers
    int rawValues[RAW_ARRAY_LENGTH], pulsePeriods[PERIOD_ARRAY_LENGTH];
    double avgValues[AVG_ARRAY_LENGTH];

    // states
    enum states_e {
      INACTIVE,
      SETTLING,
      PENDING,
      ACTIVE
    } state;
};

#endif
