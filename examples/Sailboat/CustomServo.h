/*
 * CustomServo.h
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

#ifndef CUSTOMSERVO_H_
#define CUSTOMSERVO_H_

#include "MicroGNC.h"

/*
 * Controls rotation speed and direction of DC motor by RCPWM sentences.
 */
class CustomServo: public Instrument {

  int channel;
  int pwmPin;
  int directionPin;
  int currentSensingPin;

public:
  /*
   * Constructor
   *
   * @param channel RC channel number
   * @param pwmPin PWM pin number
   * @param directionPin direction pin number
   */
  CustomServo(int channel, int pwmPin, int directionPin, int currentSensingPin);

  /*
   * Destructor
   */
  virtual ~CustomServo();

  /*
   * Reads DC motor current as MCS sentence.
   *
   * @param  sentence buffer
   * @param  maxSize buffer size. Recommended buffer size is MAX_MESSAGE_LENGTH (83 characters)
   * @return message string or NULL if no sentences are available
   */
  virtual char* getSentence(char sentence[], size_t maxSize);

  /*
    * Controls speed and direction of DC motor based on the values of
    * RCPWM sentences for the given channel.
    *
    * @param sentence sentence
    * @return true if instrument accepted the sentence
    */
   virtual bool putSentence(const char sentence[]);
};

#endif /* CUSTOMSERVO_H_ */
