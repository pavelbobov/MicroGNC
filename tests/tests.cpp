#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/Sentences/Nmea.h"
#include "../src/Instruments/CourseComputer.h"
#include "../src/Sentences/StrUtils.h"

#define GPRMC_SAMPLE                 "$GPRMC,052420.600,A,3404.3640,N,11708.7536,W,0.38,341.13,120316,,,D*7C"
#define GPWPL_SAMPLE                 "$GPWPL,3404.3640,N,11708.7529,W,TEST*4B"
#define GPBWC_SAMPLE                 "$GPBWC,220516,5130.02,N,00046.34,W,213.8,T,218.0,M,0004.6,N,EGLM*21"
//#define GPBWC_SAMPLE                 "$GPBWC,081837,,,,,,T,,M,,N,*13"

#ifdef NDEBUG
#define assert(EXPRESSION) ((void)0)
#else
#define assert(EXPRESSION) ((EXPRESSION) ? (void)0 : _assert(#EXPRESSION, __FILE__, __LINE__))
#endif

inline void _assert(const char* expression, const char* file, int line)
{
 fprintf(stdout, "Assertion '%s' failed, file '%s' line '%d'.\n", expression, file, line);
 exit(1);
}


int main( int argc, const char* argv[] )
{
  char buffer[NMEA_MAX_LENGTH];

  assert(strcmp(ftoa(12345.67890, buffer, 2), "12345.67") == 0);
  assert(strcmp(ftoa(-12345.67890, buffer, 3), "-12345.678") == 0);
  assert(strcmp(ftoa(0, buffer, 4), "0.0000") == 0);

  Point start(53.3206, -1.7297);
  Point end(53.1887,  0.1334);
  Arc arc(start, end);
  Point point(53.2611, -0.7972);
  float xte = arc.distance(point);
  assert(strcmp(ftoa(xte, buffer, 3), "-307.676") == 0);
  printf("XTE=%s\n", ftoa(xte, buffer, 3));

  CourseComputer courseComputer;
  BWCSentence bwc;

  for (int i = 0; i < 1000; i++) {
    courseComputer.putMessage(GPRMC_SAMPLE);
    courseComputer.putMessage(GPWPL_SAMPLE);
    courseComputer.getMessage(buffer, NMEA_MAX_LENGTH);
    //printf("%s\n", buffer);
    assert(bwc.set(buffer));
  }

  return 0;
}
