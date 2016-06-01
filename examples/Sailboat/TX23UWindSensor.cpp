/*
 * TX23UWindSensor.cpp
 *
 * Created on: May 27, 2016
 *     Author: pavel
 */

#include <Arduino.h>
#include "TX23UWindSensor.h"


TX23UWindSensor::TX23UWindSensor(int pin) :
Instrument("TX23U"), TxD(pin), start(millis()), requestData(false) {
}

TX23UWindSensor::~TX23UWindSensor() {
}

char* TX23UWindSensor::getSentence(char sentence[], size_t maxSize) {
  if (!requestData) { // request data
    if (millis() - start < 4000)
      return NULL;

    pinMode(TxD, OUTPUT); // pin to output
    digitalWrite(TxD, HIGH);

    delay(1);

    digitalWrite(TxD, LOW); // pull reset signal start

    requestData = true;
    start = millis();

    return NULL;
  } else {
    if (millis() - start < 500)
      return NULL;

    digitalWrite(TxD, HIGH);

    delay(15); // wait 1-2ms high, + half of the low period, approx 9ms = 11ms

    pinMode(TxD, INPUT);
    int pulse = pulseIn(TxD, HIGH, 10000) / 2;

    delayMicroseconds(pulse * 3 + pulse / 2);

    const byte MAXDATA = 36;
    bool data[MAXDATA];

    for (int pointer = 0; pointer < MAXDATA; pointer++) {
      data[pointer] = digitalRead(TxD) ? true : false;
      delayMicroseconds(pulse - 5);
    }

    requestData = false;
    start = millis();

    // validate data
    for(int i = 0; i <= 15; i++)
      if (data[i] == data[i + 20])
        return NULL;

    byte nibbles[5];
    for (byte i = 0; i < 5; i++) {
      byte n = i << 2;
      nibbles[i] = data[n + 3] << 3 | data[n + 2] << 2 | data[n + 1] << 1 | data[n];
    }

    if (nibbles[4] != ((nibbles[0] + nibbles[1] + nibbles[2] + nibbles[3]) & 0x0F))
      return NULL;

    //process data

    byte direction = data[3] << 3 | data[2] << 2 | data[1] << 1 | data[0];

    int speed = 0;
    for (int pointer = 15; pointer >= 4; pointer--)
      speed |= data[pointer] << (pointer - 4);

    MWVSentence mwv;
    mwv.windAngle = fmod(direction * 22.5 + 281.25, 360.0);
    mwv.windSpeed = speed * 0.194384; //knots

    return mwv.get(sentence, maxSize);
  }
}

