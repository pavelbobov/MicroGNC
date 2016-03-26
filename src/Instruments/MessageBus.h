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

class MessageBus;

/*
 * Base class for MessageBus instruments such as sensors, filters
 * controllers, actuators, etc.
 * 
 */
class Instrument {
protected:
  const char* id;
    
public:
  /*
   * Constructor
   * 
   * @param id instrument ID
   */
  Instrument(const char id[]) : id(id) {}

  /*
   * Destructor
   */
  virtual ~Instrument() {}
    
  /*
   * Returns instrument ID
   */
  const char* getId() { return id; }

  /*
   * Called by the message bus when the instrument is subscribed to the bus. 
   * 
   * @param bus MessageBus pointer
   */
  virtual void setMessageBus(const MessageBus* /* bus */) {}
  
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

/*
 * A bus used to exchange messages among instruments
 */
class MessageBus {
protected:
  Instrument* instruments[MAX_INSTRUMENTS];
  size_t      numInstruments = 0;
       
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
