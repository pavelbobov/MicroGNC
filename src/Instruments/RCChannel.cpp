/*
 * RCChannel.cpp
 *
 *  Created on: Jun 5, 2016
 *      Author: pavel
 */

#include <Arduino.h>
#include "RCChannel.h"
#include "../Sentences/Sentences.h"

#define PULSE_TIMEOUT 20000

inline int median(int values[]) {
  return max(min(values[0], values[1]), min(max(values[0], values[1]), values[2]));
}

RCChannel::RCChannel(int channel, int pin) :
    Instrument(TALKER_RC), channel(channel), pin(pin) {
  memset(readings, 0, sizeof(readings));
  pinMode(pin, INPUT);
}

RCChannel::~RCChannel() {
}

char* RCChannel::getSentence(char sentence[], size_t maxSize) {
  PWMSentence pwm;
  pwm.channel = channel;

  int pulse = pulseIn(pin, HIGH, PULSE_TIMEOUT);

  if (pulse == 0) {
    pwm.pulse = 0;
    return pwm.get(sentence, maxSize);
  }

  if (pulse < 900)
    return NULL;

  readings[0] = readings[1];
  readings[1] = readings[2];
  readings[2] = pulse;

  pwm.pulse = median(readings);

  return pwm.get(sentence, maxSize);
}
