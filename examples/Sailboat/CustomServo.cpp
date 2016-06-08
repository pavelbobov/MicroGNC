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

#define PWM_CENTER 1500 //microseconds
#define PWM_NOISE  10   //microseconds

CustomServo::CustomServo(int channel, int pwmPin, int directionPin) :
    Instrument("ER"), channel(channel), pwmPin(pwmPin), directionPin(directionPin) {
  pinMode(pwmPin, OUTPUT);
  pinMode(directionPin, OUTPUT);
}

CustomServo::~CustomServo() {
}

bool CustomServo::putSentence(const char sentence[]) {
  PWMSentence pwm;

  if (!pwm.set(sentence))
    return false;

  if (pwm.channel != channel)
    return false;

  if (pwm.value == 0) {
    //No RC signal
    analogWrite(pwmPin, 0);
  } else if (pwm.value > PWM_CENTER + PWM_NOISE) {
    analogWrite(pwmPin, min((pwm.value - PWM_CENTER)/2, 255));
    digitalWrite(directionPin, HIGH);
  } else if (pwm.value < PWM_CENTER - PWM_NOISE) {
    analogWrite(pwmPin, min((PWM_CENTER - pwm.value)/2, 255));
    digitalWrite(directionPin, LOW);
  } else {
    analogWrite(pwmPin, 0);
  }

  return true;
}
