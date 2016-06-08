/*
 * RCChannel.cpp
 *
 *  Created on: Jun 5, 2016
 *      Author: pavel
 */

#include <Arduino.h>
#include "../Sentences/Nmea.h"
#include "RCChannel.h"

#define PULSE_TIMEOUT 25000

//inline int median(int a, int b, int c) {
//  return max(min(a, b), min(max(a, b), c));
//}

RCChannel::RCChannel(int channel, int pin) :
    Instrument(TALKER_RC), channel(channel), pin(pin) {
  pinMode(pin, INPUT);
}

RCChannel::~RCChannel() {
}

char* RCChannel::getSentence(char sentence[], size_t maxSize) {
  PWMSentence pwm;
  pwm.channel = channel;
  pwm.value = pulseIn(pin, HIGH, PULSE_TIMEOUT);

  if (pwm.value != 0 && pwm.value < 900)
    return NULL;

  return pwm.get(sentence, maxSize);
}
