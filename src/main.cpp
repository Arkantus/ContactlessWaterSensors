#include <Arduino.h>

const int sensors = 3;
const int leds = 3;

const int sensorPins[sensors] = {4, 5, 7};
const int ledPins[leds] = {19, 20, 21};

bool sensorsReads[sensors]; //status of the sensors
bool ledStatus[leds]; //status of the leds

int triggeredSensors = 0; //number of triggered sensors

enum led_mode {defaultOn, defaultOff};

bool use_mode(bool status, led_mode mode)
{
  if (mode == defaultOff)
  {
    return status;
  }
  else
  {
    return !status;
  }
  
}

const led_mode mode = defaultOn;

void setup() 
{
  ADCSRA &= ~(1<<ADEN); //disable ADC for power saving

  for(int i = 0; i < sensors; i++)
  {
    pinMode(sensorPins[i],INPUT);
    sensorsReads[i] = false;
  } //initialize sensors to input, set status to false (not triggered)

  for(int i = 0; i < leds; i++)
  {
    pinMode(ledPins[i], OUTPUT);
    ledStatus[i] = use_mode(HIGH, mode);
    digitalWrite(ledPins[i], ledStatus[i]);
  }//initalize led to ouput, write default value
}

void loop() 
{
  triggeredSensors = 0;
  for(int i = 0; i < sensors; i++)
  {
    bool reading = digitalRead(sensorPins[i]);
    sensorsReads[i] = reading;
    if (reading == false)
    {
      triggeredSensors++;
    }
  } //count triggered sensors
  //This only count, and does not take in account the order in which the sensors have been triggered, (ie waterline 2 trips before 1, this could be used to uncover a potential problem)

  for( int i = 0 ; i < triggeredSensors ; i++)
  {
    ledStatus[i] = use_mode(LOW, mode);
  }//modify led status to correspond

  for( int i = 0 ; i < leds ; i++ )
  {
    int newOderLedPins = leds -1 - i; 
    digitalWrite(ledPins[newOderLedPins], ledStatus[i]);
    ledStatus[i] = use_mode(HIGH, mode);
  }//write status to leds, reset status to default;
}