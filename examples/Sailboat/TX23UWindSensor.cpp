/*
 * TX23UWindSensor.cpp
 *
 * Driver for LaCrosse TX-23U wind sensor.
 *
 * (C) Copyright 2016 Pavel Bobov.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <Arduino.h>
#include "TX23UWindSensor.h"

inline byte nibble(bool* data, byte n) {
  return data[n + 3] << 3 | data[n + 2] << 2 | data[n + 1] << 1 | data[n];
}

TX23UWindSensor::TX23UWindSensor(int pin) :
  Instrument("TX23U"), TxD(pin), start(millis()), requestData(false) {
}

TX23UWindSensor::~TX23UWindSensor() {
}

char* TX23UWindSensor::getSentence(char sentence[], size_t maxSize) {
  if (!requestData) { // request data
    if (millis() - start < 4000)
      return NULL;

    pinMode(TxD, OUTPUT);
    digitalWrite(TxD, HIGH);

    delay(1);

    digitalWrite(TxD, LOW); // pull reset signal start

    requestData = true;
    start = millis();

    return NULL;
  } else {
    if (millis() - start < 500)
      return NULL;

    requestData = false;
    start = millis();

    pinMode(TxD, OUTPUT);
    digitalWrite(TxD, HIGH);

    delay(16); // wait 1-2ms high, + half of the low period, approx 9ms = 11ms

    pinMode(TxD, INPUT);
    int pulse = pulseIn(TxD, HIGH, 10000) / 2;

    delayMicroseconds(pulse * 3 + pulse / 2);

    bool data[20];
    for (byte pointer = 0; pointer < 36; pointer++) {
      bool b = digitalRead(TxD) ? 1 : 0;

      if (pointer < 20)
        data[pointer] = b;
      else if (data[pointer - 20] == b)
        return NULL;

      delayMicroseconds(pulse - 5);
    }

    // validate data
    byte checksum = 0;
    for (byte i = 0; i < 4; i++)
      checksum += nibble(data, i << 2);

    if (nibble(data, 16) != (checksum & 0x0F))
      return NULL;

    //process data
    unsigned int speed = 0;
    for (byte pointer = 15; pointer >= 4; pointer--)
      speed |= data[pointer] << (pointer - 4);

    MWVSentence mwv;
    mwv.windAngle = fmod(nibble(data, 0) * 22.5 + 281.25, 360.0);
    mwv.windSpeed = speed * 0.194384; //knots

    return mwv.get(sentence, maxSize);
  }
}
