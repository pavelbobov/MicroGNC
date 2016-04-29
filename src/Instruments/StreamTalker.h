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
 * Writes/reads sentences to/from the specified stream.
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
   * If filters are added, sentences that do not start with any of the filters
   * are ignored by putSentence() method.
   * 
   * @param filter filter string
   */
  void addFilter(const char filter[]);

  /*
   * Reads sentence from the stream.
   * 
   * @param sentence buffer
   * @param buflen buffer size
   * @return message
   */
  char* getSentence(char sentence[], size_t buflen);

  /*
   * Writes sentence to the stream.
   * 
   * @param sentence sentence to write
   * @return true if the sentence was successfully written
   */
  bool  putSentence(const char sentence[]);

private:
  bool  filter(const char sentence[]) const;
};

#endif /* _STREAMTALKER_H_ */
