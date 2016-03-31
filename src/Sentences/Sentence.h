/*
 * Sentence.h
 * 
 * Base class for sentences.
 * 
 * The specification of NMEA sentences is based on publicly available sources,
 * such as http://www.catb.org/gpsd/NMEA.html. 
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
 
#ifndef _SENTENCE_H_
#define _SENTENCE_H_

#include <stdlib.h>

/*
 * Base class for sentences
 * 
 */
class Sentence {
public:
  /*
   * Constructor
   * 
   * @param talker talker ID
   * @param tag tag
   */

  Sentence(const char talker[], const char tag[]);
  /*
   * Destructor
   */
  virtual ~Sentence();

  /*
   * Implementations of this method must copy sentence to the specified buffer
   * 
   * @param buffer buffer
   * @param buflen buffer size.
   * @return sentence or NULL if the buffer size is insufficient.
   */
  virtual char* get(char buffer[], size_t buflen) const = 0;

  /*
   * Implementations of this method must change the sentence to the specified string
   * if the string is a valid sentence and the sentence tag matches the tag of the string.
   * 
   * @param str sentence
   * @return true if the sentence was changed
   */
  virtual bool  set(const char str[]) = 0;

protected:
  /*
   * Talker ID - a two-character prefix that identifies 
   * the type of the transmitting unit. 
   * 
   * "P" for proprietary NMEA tags
   */
  const char* talker;

  /*
   * Sentence tag - a three character string that
   * identifies the type of sentence.
   */
  const char* tag;

  static bool  valid(const char str[]);
  bool         matches(const char str[]);

  /*
   * Calculates sentence checksum and adds it to the end of the string
   */
  static char* addChecksum(char str[]);
};

/*
 * Converts numbers 0-15 to hex digit characters '0'-'F'
 *
 * @param c number 0-15
 * @return hex digit character '0'-'F'
 */
char toHex(unsigned char c);

/*
 * Converts hex digit characters '0'-'F' to numbers 0-15
 *
 * @param c hex digit character '0'-'F'
 * @return number 0-15
 */
unsigned char parseHex(char c);

#endif /* _SENTENCE_H_ */

