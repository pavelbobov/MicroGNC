/*
 * MessageBus.cpp
 * 
 * Message Bus and device interface.
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

#include "string.h" 
#include "MessageBus.h"

/************************* NMEAMessageBus ***********************************/

MessageBus::MessageBus() {
}

Instrument* MessageBus::getInstrumentById(char* id) {
  for (unsigned char i = 0; i < getNumInstruments(); i++)
    if (strcmp(instruments[i]->getId(), id) == 0)
      return instruments[i]; 

  return NULL;  
}
    
/*
 * Subscribes talker to the bus.
 */
bool MessageBus::subscribe(Instrument* instrument) {
  if (instrument == NULL || numInstruments >= MAX_INSTRUMENTS)
    return false;
    
  instruments[numInstruments++] = instrument; 

  return true;
}

/*
 * Broadcasts message to all talkers in the bus except for the sender.
 */
void MessageBus::broadcast(const char message[], const char senderId[]) {
  for (unsigned char receiver = 0; receiver < numInstruments; receiver++) 
    if (strcmp(instruments[receiver]->getId(), senderId) != 0)
      instruments[receiver]->putMessage(message);
}

/*
* Exchanges messages among talkers 
*/ 
void MessageBus::exchange() {
  char buffer[MAX_MESSAGE_LENGTH];
  
  for (unsigned char sender = 0; sender < numInstruments; sender++) {
    const char* message = instruments[sender]->getMessage(buffer, MAX_MESSAGE_LENGTH);

    if (message)
      broadcast(message, instruments[sender]->getId());
  }
}

