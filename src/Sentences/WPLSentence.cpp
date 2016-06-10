#include <string.h>

#include "Sentences.h"
#include "StrUtils.h"
  
WPLSentence::WPLSentence() : Sentence(TALKER_GP, TAG_WPL) {
  name[0] = '\0';
}
  
WPLSentence::~WPLSentence() {
}

char* WPLSentence::get(char str[], size_t buflen) const {
  if (str == NULL || buflen < MAX_SENTENCE_LENGTH)
    return NULL;
    
  addHead(str);

  strcat(str, ",");

  char* p = strend(str);

  pointToString(waypoint, p);

  strcat(p, ",");

  strcat(p, name);
  
  return addChecksum(str);
}

bool WPLSentence::set(const char str[]) {
  if (!valid(str))
    return false;

  if (!matches(str))
    return false;
  
  const char *p = str;

  // get time
  p = nextToken(p);
  
  p = parsePoint(p, waypoint);

  if (',' != *p)
    strncpy(name, p, strchr(p, '*') - p);

  return true;
}

