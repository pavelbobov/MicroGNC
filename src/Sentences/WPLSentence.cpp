#include <string.h>
#include "Nmea.h"
#include "StrUtils.h"
  
WPLSentence::WPLSentence() : Sentence(TALKER_GP, NMEA_WPL) {
  name[0] = '\0';
}
  
WPLSentence::~WPLSentence() {
}

char* WPLSentence::get(char str[], size_t buflen) const {
  if (str == NULL || buflen < NMEA_MAX_LENGTH)
    return NULL;
    
  addHead(str);

  strcat(str, ",");

  char* p = strend(str);

  pointToString(waypoint, p);

  strcat(p, ",");

  strcat(p, name);
  
  return addChecksum(str);
}

bool WPLSentence::set(const char nmea[]) {
  if (!valid(nmea))
    return false;

  if (!matches(nmea))
    return false;
  
  const char *p = nmea;

  // get time
  p = nextToken(p);
  
  p = parsePoint(p, waypoint);

  if (',' != *p)
    strncpy(name, p, strchr(p, '*') - p);

  return true;
}

