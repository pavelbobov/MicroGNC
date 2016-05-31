#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/Sentences/Nmea.h"
#include "../src/Instruments/CourseComputer.h"
#include "../src/Sentences/StrUtils.h"

#define GPRMC_SAMPLE "$GPRMC,052420.600,A,3404.3640,N,11708.7536,W,0.38,341.13,120316,,,D*7C"
#define GPWPL_SAMPLE "$GPWPL,3404.3640,N,11708.7529,W,TEST*4B"
#define GPBWC_SAMPLE "$GPBWC,220516,5130.02,N,00046.34,W,213.8,T,218.0,M,0004.6,N,EGLM*21"
//#define GPBWC_SAMPLE                 "$GPBWC,081837,,,,,,T,,M,,N,*13"
#define WIMWV_SAMPLE "$WIMWV,214.8,R,0.1,K,A*28"

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

  { // ftoa
    assert(strcmp(ftoa(12345.67890, buffer, 2), "12345.67") == 0);
    assert(strcmp(ftoa(-12345.67890, buffer, 3), "-12345.678") == 0);
    assert(strcmp(ftoa(0, buffer, 4), "0.0000") == 0);
  }

  { // XTE distance
    Point start(53.3206, -1.7297);
    Point end(53.1887,  0.1334);
    Arc arc(start, end);
    Point point(53.2611, -0.7972);
    float xte = arc.distance(point);
    assert(strcmp(ftoa(xte, buffer, 3), "-307.676") == 0);
  }

  { // length
    Point p1(90, 0);
    Point p2(-90, 0);
    Arc arc(p1, p2);
    assert(strcmp(ftoa(arc.length(), buffer, 1), "20015116.0") == 0);
  }

  { // bearing
    Point p1(52.205, 0.119);
    Point p2(48.857, 2.351);
    Arc arc(p1, p2);
    assert(strcmp(ftoa(arc.bearing(), buffer, 1), "156.1") == 0);
  }

  CourseComputer courseComputer;
  BWCSentence bwc;

  for (int i = 0; i < 1000; i++) {
    courseComputer.putSentence(GPRMC_SAMPLE);
    courseComputer.putSentence(GPWPL_SAMPLE);
    courseComputer.getSentence(buffer, NMEA_MAX_LENGTH);
    //printf("%s\n", buffer);
    assert(bwc.set(buffer));
  }

  {
    MWVSentence mwv;
    mwv.set(WIMWV_SAMPLE);
    assert(strcmp(ftoa(mwv.windAngle, buffer, 1), "214.8") == 0);
    assert(mwv.reference == 'R');
    assert(strcmp(ftoa(mwv.windSpeed, buffer, 1), "0.1") == 0);
    assert(mwv.windSpeedUnits == 'K');
    mwv.windAngle = 123.4;
    mwv.reference = 'R';
    mwv.windSpeed = 12.3;
    mwv.windSpeedUnits = 'K';
    mwv.get(buffer, NMEA_MAX_LENGTH);
    assert(strcmp(buffer, "$WIMWV,123.4,R,12.3,K,A*12") == 0);
  }

  return 0;
}
