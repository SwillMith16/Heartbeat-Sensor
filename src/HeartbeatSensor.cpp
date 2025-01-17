#include "HeartbeatSensor.h"


/********************************
  Function: begin
  Description: Used to initialise values that don't need to be public
  Parameters: none
  Returns: none
********************************/
void HeartbeatSensor::begin(double limit){
  rawValues[RAW_ARRAY_LENGTH] = {0};
  avgValues[AVG_ARRAY_LENGTH] = {0};
  pulsePeriods[PERIOD_ARRAY_LENGTH] = {0};
  rawIndex = 0;
  avgIndex = 0;
  periodIndex = 0;
  pulseDetected = false;
  pulseTime = 0;
  lastPulseTime = 0;
  bpm = 0;
  detectionLimit = limit;
}


/********************************
  Function: getArrayAvg
  Description: calculates the average value of an int array
  Parameters:  int array, int array length
  Returns: double
********************************/
double HeartbeatSensor::getArrayAvg(int array[], int arrayLength){
  int sum = 0;
  double avg = 0.0;

  for(int i = 0; i < arrayLength; i++){
    sum = sum + array[i];
    avg = sum / (arrayLength);
  }

  return avg;
}

/********************************
  Function: getArrayAvg
  Description: function overload for compatability with double type arrays
  Parameters:  double array, int array length
  Returns: double
********************************/
double HeartbeatSensor::getArrayAvg(double array[], int arrayLength){
  double sum = 0.0;
  double avg = 0.0;

  for(int i = 0; i < arrayLength; i++){
    sum = sum + array[i];
    avg = sum / (arrayLength);
  }

  return avg;
}


/********************************
  Function: measureData
  Description: populates arrays with measured data and derived data for plotting
  Parameters: none
  Returns: none
********************************/
void HeartbeatSensor::measureData(){
  // measure data
  rawValues[rawIndex] = value;
  rawIndex++;
  if (rawIndex == RAW_ARRAY_LENGTH) rawIndex = 0;

  // get avg of data samples
  avgValue = getArrayAvg(rawValues, RAW_ARRAY_LENGTH);

  // add to array of averages
  avgValues[avgIndex] = avgValue;
  avgIndex++;
  if (avgIndex == AVG_ARRAY_LENGTH) avgIndex = 0;

  // use array of averages to get a consistent average baseline
  avgBaseline = getArrayAvg(avgValues, AVG_ARRAY_LENGTH);

  if (avgValue > avgBaseline + detectionLimit && !pulseDetected) { // pulse detected
    pulseDetected = 1;
    pulseTime = millis();

    pulsePeriods[periodIndex] = pulseTime - lastPulseTime;
    periodIndex++;
    if (periodIndex == PERIOD_ARRAY_LENGTH) periodIndex = 0;
    
    lastPulseTime = pulseTime;
  }
  else if (avgValue < avgBaseline + detectionLimit) pulseDetected = 0;

  avgPulsePeriod = getArrayAvg(pulsePeriods, PERIOD_ARRAY_LENGTH);
  // avgPulsePeriod is in milliseconds. Divide by 1k to get seconds
  // 1 over this gives frequency (per second). multiply by 60 for per minute 
  bpm = (1 / (avgPulsePeriod/1000)) * 60;
}
