/*
 * CustomServo.cpp
 *
 * Controls DC motor by RCPWM sentences
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
#include "CustomServo.h"

float volt_per_amp = 1.65; // resolution according to hardware page

CustomServo::CustomServo(int channel, int pwmPin, int directionPin, int currentSensingPin) :
    Instrument(TALKER_ER), channel(channel), pwmPin(pwmPin), directionPin(directionPin),
    currentSensingPin(currentSensingPin) {
  pinMode(pwmPin, OUTPUT);
  pinMode(directionPin, OUTPUT);
}

CustomServo::~CustomServo() {
}

char* CustomServo::getSentence(char sentence[], size_t maxSize) {
  MCSSentence mcs;
  mcs.motor = channel;
  float currentRaw = analogRead(currentSensingPin);
  float currentVolts = currentRaw * (5.0 / 1024.0);
  mcs.current = currentVolts / volt_per_amp;
  return mcs.get(sentence, maxSize);
}

bool CustomServo::putSentence(const char sentence[]) {
  PWMSentence pwm;

  if (!pwm.set(sentence))
    return false;

  if (pwm.channel != channel)
    return false;

  if (pwm.pulse == 0) {
    //No RC signal
    analogWrite(pwmPin, 0);
  } else if (pwm.pulse > PWM_CENTER + PWM_NOISE) {
    analogWrite(pwmPin, min((pwm.pulse - PWM_CENTER)/2, 255));
    digitalWrite(directionPin, HIGH);
  } else if (pwm.pulse < PWM_CENTER - PWM_NOISE) {
    analogWrite(pwmPin, min((PWM_CENTER - pwm.pulse)/2, 255));
    digitalWrite(directionPin, LOW);
  } else {
    analogWrite(pwmPin, 0);
  }

  return true;
}
