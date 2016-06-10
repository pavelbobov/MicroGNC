/*
 * RCChannel.cpp
 *
 *  Created on: Jun 5, 2016
 *      Author: pavel
 */

#include <Arduino.h>
#include "RCChannel.h"
#include "../Sentences/Sentences.h"

#define PULSE_TIMEOUT 25000

inline int median(int a, int b, int c) {
  return max(min(a, b), min(max(a, b), c));
}

RCChannel::RCChannel(int channel, int pin) :
    Instrument(TALKER_RC), channel(channel), pin(pin) {
  pinMode(pin, INPUT);
}

RCChannel::~RCChannel() {
}

char* RCChannel::getSentence(char sentence[], size_t maxSize) {
  PWMSentence pwm;
  pwm.channel = channel;

  int a = pulseIn(pin, HIGH, PULSE_TIMEOUT);

  if (a == 0) {
    pwm.pulse = 0;
    return pwm.get(sentence, maxSize);
  }

  if (a < 900)
    return NULL;

  delay(10);

  int b = pulseIn(pin, HIGH, PULSE_TIMEOUT);

  if (b == 0) {
    pwm.pulse = 0;
    return pwm.get(sentence, maxSize);
  }

  if (b < 900)
    return NULL;

  delay(10);

  int c = pulseIn(pin, HIGH, PULSE_TIMEOUT);

  if (c == 0) {
    pwm.pulse = 0;
    return pwm.get(sentence, maxSize);
  }

  if (c < 900)
    return NULL;

  pwm.pulse = median(a,b,c);

  return pwm.get(sentence, maxSize);
}
