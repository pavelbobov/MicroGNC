#include <stdio.h>
#include <stdlib.h>
#include "../src/Sentences/Nmea.h"
#include "../src/Instruments/CourseComputer.h"

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

CourseComputer courseComputer;

int main( int argc, const char* argv[] )
{
  char buffer[NMEA_MAX_LENGTH];
  courseComputer.putMessage(GPRMC_SAMPLE);
  courseComputer.putMessage(GPWPL_SAMPLE);
  courseComputer.getMessage(buffer, NMEA_MAX_LENGTH);
  printf("%s\n", buffer);
  BWCSentence bwc;
  assert(bwc.set(buffer));

  return 0;
}
