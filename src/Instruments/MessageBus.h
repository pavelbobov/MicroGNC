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
 
#ifndef _MESSAGEBUS_H_
#define _MESSAGEBUS_H_

#define MAX_INSTRUMENTS     10
#define MAX_MESSAGE_LENGTH  83

#include <stdlib.h>
#include "Instrument.h"

/*
 * A bus used to exchange messages among instruments
 */
class MessageBus {
protected:
  Instrument* instruments[MAX_INSTRUMENTS];
  size_t      numInstruments;
       
public:
  /*
   * Constructor
   */
  MessageBus();

  /*
   * Returns an array of instruments
   * 
   * @return array of pointers to Instrument 
   */
  Instrument**  getInstruments() { return instruments; }

   /*
    * Returns the number of instruments subscribed to the message bus
    */
  size_t        getNumInstruments() { return numInstruments; }

  /*
   * Returns the first subscribed instrument with given ID
   * 
   * @param id instrument ID
   * @return instrument
   */
  Instrument*   getInstrumentById(char* id);
  
  /*
   * Adds instrument to the end of the list of instruments. 
   * The order is important.
   * 
   * The message bus self pointer is passed to the instrument 
   * by calling setMessageBus(this) method.
   * 
   * @param instrument instrument
   * @return true if instrument was successfully subscribed to the bus
   */
  bool subscribe(Instrument* instrument);
  
  /*
   * Broadcasts message to all instruments in the bus except for sender.
   * 
   * @param message message
   * @param senderId ID of the instrument that sent the message
   */
  void broadcast(const char message[], const char senderId[]);

  /*
   * Exchanges messages among instruments. 
   * For each instrument, gets message from the instrument and
   * broadcasts the message to all the other instruments.
   */ 
  void exchange();    
};

#endif /* _MESSAGEBUS_H_ */
