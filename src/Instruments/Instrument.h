/*
 * MessageBus.h
 * 
 * Message bus and base class for instruments.
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
 
#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_

#include <stdlib.h>

/*
 * Base class for instruments such as sensors, filters
 * controllers, actuators, etc.
 * 
 */
class Instrument {
protected:
  const char* id;

  /*
   * Constructor
   * 
   * @param id instrument ID
   */
  Instrument(const char id[]) : id(id) {}

public:
  /*
   * Destructor
   */
  virtual ~Instrument() {}
    
  /*
   * Returns instrument ID
   */
  const char* getId() { return id; }

  /*
   * Gets message from instrument
   * 
   * @param  message message buffer
   * @param  maxSize message buffer size. Recommended buffer size is MAX_MESSAGE_LENGTH (83 caracters)
   * @return message string or NULL if no messages are available
   */
  virtual char* getMessage(char[] /* message */, size_t /* maxSize */) { return NULL; }

  /*
   * Puts message to the instrument
   * 
   * @param  message message
   * @return true if instrument accepted the message
   */
  virtual bool putMessage(const char[] /* message */) { return false; }
};

#endif /* _INSTRUMENT_H_ */
