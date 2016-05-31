/*
 * TX23UWindSensor.cpp
 *
 * Created on: May 27, 2016
 *     Author: pavel
 */

#include <Arduino.h>
#include "TX23UWindSensor.h"


TX23UWindSensor::TX23UWindSensor(int pin) :
Instrument("TX23U"), TxD(pin), start(millis()) {
}

TX23UWindSensor::~TX23UWindSensor() {
}

char* TX23UWindSensor::getSentence(char sentence[], size_t maxSize) {
  if (millis() - start < 5000) {
    return NULL;
  }

  start = millis();

  // request data

  pinMode(TxD, OUTPUT); // pin to output
  digitalWrite(TxD, HIGH);

  delay(1);

  digitalWrite(TxD, LOW); // pull reset signal start

  delay(500); //wait reset signal length

  digitalWrite(TxD, HIGH);

  delay(15); // wait 1-2ms high, + half of the low period, approx 9ms = 11ms

  pinMode(TxD, INPUT);
  int pulse1 = pulseIn(TxD, HIGH, 10000);
  int samplingDelay = pulse1 / 6 - 5;
  delayMicroseconds(pulse1 / 2 * 3 + samplingDelay / 6);

  static const byte MAXDATA = 36;

  boolean data[MAXDATA];
  memset(data, 0, sizeof(data));

  for (int pointer = 0; pointer < MAXDATA; pointer++) {
    for (byte i = 0; i < 3; i++) {
      data[pointer] = data[pointer] || digitalRead(TxD);
      delayMicroseconds(samplingDelay);
    }
  }

  // validate data

  for(int i = 0; i <= 15; i++) {
    if (data[i] != !data[i + 20]) {
      strcpy(sentence, "$WIMWV,BAD3*00");
      return sentence;
      //return NULL;
    }
  }

  byte nibbles[5];
  for (int i=0; i<5; i++) {
    nibbles[i] =0;

    nibbles[i] = data[i * 4 + 3] << 3 | data[i * 4 + 2] << 2 |
                 data[i * 4 + 1] << 1 | data[i * 4 + 0];
  }

  if (nibbles[4] != ((nibbles[0] + nibbles[1] + nibbles[2] + nibbles[3]) & 0x0F)) {
    strcpy(sentence, "$WIMWV,BAD4*00");
    return sentence;
    //return NULL;
  }

  //process data

  byte direction = data[3] << 3 | data[2] << 2 | data[1] << 1 | data[0];

  int speed = 0;
  for (int pointer = 15; pointer >= 4; pointer--) {
    speed |= data[pointer] << (pointer - 4);
  }

  MWVSentence mwv;
  mwv.windAngle = fmod(direction * 22.5 + 281.25, 360.0);
  mwv.windSpeed = speed * 0.194384; //knots

  return mwv.get(sentence, maxSize);
}

