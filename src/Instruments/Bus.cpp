/*
 * Bus.cpp
 * 
 * Bus used to exchange sentence among instruments.
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

#include "Bus.h"

/********************************* Bus ****************************************/

Bus::Bus() : numInstruments(0) {
}

Instrument* Bus::getInstrumentById(char* id) {
  for (unsigned char i = 0; i < getNumInstruments(); i++)
    if (strcmp(instruments[i]->getId(), id) == 0)
      return instruments[i]; 

  return NULL;  
}
    
/*
 * Subscribes talker to the bus.
 */
bool Bus::subscribe(Instrument* instrument) {
  if (instrument == NULL || numInstruments >= MAX_INSTRUMENTS)
    return false;
    
  instruments[numInstruments++] = instrument; 

  return true;
}

/*
 * Broadcasts sentence to all talkers in the bus except for the sender.
 */
void Bus::broadcast(const char sentence[], const char senderId[]) {
  for (unsigned char receiver = 0; receiver < numInstruments; receiver++) 
    if (strcmp(instruments[receiver]->getId(), senderId) != 0)
      instruments[receiver]->putSentence(sentence);
}

/*
* Exchanges messages among talkers 
*/ 
void Bus::exchange() {
  char buffer[MAX_SENTENCE_LENGTH];
  
  for (unsigned char sender = 0; sender < numInstruments; sender++) {
    const char* sentence = instruments[sender]->getSentence(buffer, MAX_SENTENCE_LENGTH);

    if (sentence)
      broadcast(sentence, instruments[sender]->getId());
  }
}

