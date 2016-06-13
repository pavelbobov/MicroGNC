/*
 * RCChannel.h
 *
 *  Created on: Jun 5, 2016
 *      Author: pavel
 */

#ifndef RCCHANNEL_H_
#define RCCHANNEL_H_

#include "Instrument.h"

#define PWM_CENTER 1500 //microseconds
#define PWM_NOISE  20   //microseconds

/*
 * Reads input from radio controller receiver
 */
class RCChannel: public Instrument {

  int channel;
  int pin;
  int readings[3];

public:
  /*
   * Constructor
   *
   * @param channel RC channel number
   * @param pin digital pin connected to the RC receiver channel
   */
  RCChannel(int channel, int pin);

  /*
   * Destructor
   */
  virtual ~RCChannel();

  /*
   * Reads input from radio controller receiver formatted as PWM sentence.
   *
   * @param  sentence buffer
   * @param  maxSize buffer size. Recommended buffer size is MAX_MESSAGE_LENGTH (83 characters)
   * @return message string or NULL if no sentences are available
   */
  virtual char* getSentence(char sentence[], size_t maxSize);
};

#endif /* RCCHANNEL_H_ */
