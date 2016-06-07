/*
 * RCChannel.cpp
 *
 *  Created on: Jun 5, 2016
 *      Author: pavel
 */

#include <Arduino.h>
#include "../Sentences/Nmea.h"
#include "RCChannel.h"

#define PULSE_TIMEOUT 3000

RCChannel::RCChannel(int channel, int pin) :
  Instrument("RC"), channel(channel), pin(pin) {
  pinMode(pin, INPUT);
}

RCChannel::~RCChannel() {
}

char* RCChannel::getSentence(char sentence[], size_t maxSize) {
  PWMSentence pwm;
  pwm.channel = channel;
  pwm.value = pulseIn(pin, HIGH, PULSE_TIMEOUT);
  return pwm.get(sentence, maxSize);
}
