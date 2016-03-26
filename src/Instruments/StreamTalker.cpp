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

char* StreamTalker::getMessage(char message[], size_t buflen) {
  if (!stream->available())
    return NULL;

  message[0] = '\0';
  
  for (size_t i = 0; i < buflen; i++) {
    message[0] = stream->read();
    
    if (message[0] == START_CHAR || message[0] == -1) 
      break;
  }

  if (message[0] != START_CHAR)
    return NULL;
  
  size_t len = stream->readBytesUntil(END_CHAR, message + 1, buflen - 1);

  if (len == 0)
    return NULL;

  message[++len] = '\0';

  return message;
}

bool StreamTalker::putMessage(const char message[]) {
  if (!filter(message)) 
    return false;
  
  stream->println(message);
  
  return true;
}

bool StreamTalker::filter(const char message[]) const {
  if (numFilters == 0) return true;
  
  for (size_t i = 0; i < numFilters; i++) 
    if (strncmp(message, filters[i], strlen(filters[i])) == 0) 
      return true;

  return false;
}

