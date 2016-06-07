/*
 * RCChannel.h
 *
 *  Created on: Jun 5, 2016
 *      Author: pavel
 */

#ifndef RCCHANNEL_H_
#define RCCHANNEL_H_

#include "Instrument.h"

/*
 * Reads input from radio controller receiver
 */
class RCChannel: public Instrument {

  int channel;
  int pin;

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
   * Reads input from radio controller receiver formatted as RCH NMEA sentence.
   *
   * @param  sentence buffer
   * @param  maxSize buffer size. Recommended buffer size is MAX_MESSAGE_LENGTH (83 characters)
   * @return message string or NULL if no sentences are available
   */
  virtual char* getSentence(char sentence[], size_t maxSize);
};

#endif /* RCCHANNEL_H_ */
