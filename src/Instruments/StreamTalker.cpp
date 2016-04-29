/*
 * StreamTalker.cpp
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

#include "StreamTalker.h"

#define START_CHAR '$'
#define END_CHAR   '\n'


StreamTalker::StreamTalker(const char id[], Stream* stream) :
  Instrument(id), stream(stream), numFilters(0) {
}

void StreamTalker::addFilter(const char filter[]) {
  filters[numFilters++] = filter;
}

char* StreamTalker::getSentence(char sentence[], size_t buflen) {
  if (!stream->available())
    return NULL;

  sentence[0] = '\0';
  
  for (size_t i = 0; i < buflen; i++) {
    sentence[0] = stream->read();
    
    if (sentence[0] == START_CHAR || sentence[0] == -1) 
      break;
  }

  if (sentence[0] != START_CHAR)
    return NULL;
  
  size_t len = stream->readBytesUntil(END_CHAR, sentence + 1, buflen - 1);

  if (len == 0)
    return NULL;

  sentence[++len] = '\0';

  return sentence;
}

bool StreamTalker::putSentence(const char sentence[]) {
  if (!filter(sentence)) 
    return false;
  
  stream->println(sentence);
  
  return true;
}

bool StreamTalker::filter(const char sentence[]) const {
  if (numFilters == 0) return true;
  
  for (size_t i = 0; i < numFilters; i++) 
    if (strncmp(sentence, filters[i], strlen(filters[i])) == 0) 
      return true;

  return false;
}

