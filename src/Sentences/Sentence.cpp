/*
 * NMEASentence.cpp
 * 
 * NMEA sentences parsing and construction.
 * 
 * http://www.catb.org/gpsd/NMEA.html 
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

#include <string.h>
#include "Sentence.h"

/*
 * Converts numbers 0-15 to hex digit characters '0'-'F'
 *
 * @param c number 0-15
 * @return hex digit character '0'-'F'
 */
char toHex(unsigned char c) {
  return c < 10 ? '0' + c : 'A' + c - 10;
}

/*
 * Converts hex digit characters '0'-'F' to numbers 0-15
 *
 * @param c hex digit character '0'-'F'
 * @return number 0-15
 */
unsigned char parseHex(char c) {
  if (c < '0')
    return 0;
  if (c <= '9')
    return c - '0';
  if (c < 'A')
    return 0;
  if (c <= 'F')
    return (c - 'A') + 10;
  // if (c > 'F')
  return 0;
}

/********************************** Sentence **********************************/

Sentence::Sentence(const char talker[], const char tag[]) : 
  talker(talker), tag(tag) {
}

Sentence::~Sentence() {
}

bool Sentence::matches(const char str[]) {
  return strncmp(str + 1 + strlen(talker), tag, strlen(tag)) == 0;
}

bool Sentence::valid(const char str[]) {
  if (!str)
    return false;
    
  unsigned char sum = 0;
  const char* p;
  
  for (p = str + 1; *p != '*' && *p != '\0'; p++)
    sum ^= *p;

  if (*p == '\0') return false;

  sum ^= (parseHex(p[1]) << 4) + parseHex(p[2]);

  return !sum;
}

char* Sentence::addHead(char str[]) const {
  str[0] = '$';
  str[1] = '\0';
  strcat(str, talker);
  strcat(str, tag);
  return str;
}

char* Sentence::addChecksum(char str[]) {
  unsigned char parity = 0;

  char* p = str;
  
  for(p++; *p != 0; p++)
    parity ^= *p; 

  p[0] = '*';
  p[1] = toHex(parity / 16);
  p[2] = toHex(parity % 16);
  p[3] = '\0';
  
  return str;
}
