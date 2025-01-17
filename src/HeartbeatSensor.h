#ifndef HEARTBEAT_SENSOR_H
#define HEARTBEAT_SENSOR_H

#include <Arduino.h>

const int RAW_ARRAY_LENGTH = 10;
const int AVG_ARRAY_LENGTH = 100;
const int PERIOD_ARRAY_LENGTH = 10;

class HeartbeatSensor
{
  private:
    int rawIndex, avgIndex, periodIndex, pulseTime, lastPulseTime, pulsePeriod;
    bool pulseDetected;

  public:
    // function declarations
    double getArrayAvg(int array[], int arrayLength);
    double getArrayAvg(double array[], int arrayLength);
    void begin(double limit), measureData();

    // variables
    double avgValue, avgBaseline, avgPulsePeriod, detectionLimit;
    int value, bpm;

    // containers
    int rawValues[RAW_ARRAY_LENGTH], pulsePeriods[PERIOD_ARRAY_LENGTH];
    double avgValues[AVG_ARRAY_LENGTH];
};

#endif
