/*
 * StreamTalker.h
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

#ifndef _STREAMTALKER_H_
#define _STREAMTALKER_H_

#include <Stream.h>
#include "Instrument.h"

#define MAX_STREAM_FILTERS 10

/*
 * Sends/receives messages to/from the specified stream.
 */
class StreamTalker : public Instrument {
protected:
  Stream*     stream;
  const char* filters[MAX_STREAM_FILTERS];
  size_t      numFilters = 0;
  
public:
  /*
   * Constructor
   * 
   * @param id talker ID ("GP" for GPS)
   * @param stream pointer to a Stream subclass, such as Serial or SoftwareSerial
   */
  StreamTalker(const char id[], Stream* stream);

  /*
   * Adds filter to the list of filters.
   * If filters are added, messages that do not start with any of the filters 
   * are ignored by putMessage() method.
   * 
   * @param filter filter string
   */
  void addFilter(const char filter[]);

  /*
   * Reads message from the stream.
   * 
   * @param message message buffer
   * @param buflen buffer size
   * @return message
   */
  char* getMessage(char message[], size_t buflen);

  /*
   * Writes message to the stream.
   * 
   * @param message message to write
   * @return true if the message was successfully written
   */
  bool  putMessage(const char message[]);

private:
  bool  filter(const char message[]) const; 
};

#endif /* _STREAMTALKER_H_ */
