/*
 * Sailboat.ino
 * 
 * Example Arduino sketch for MicroGNC library
 *
 * Radio controlled boat that sends GPRMC GPS sentences to serial port cinnected
 * to a 915MHz radio-telemetry air module. 
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
 
#include <SoftwareSerial.h>
#include "MicroGNC.h"
#include "TX23UWindSensor.h"
#include "CustomServo.h"

//Analog pins
#define RUDDER_CURRENT  A0 
#define WINCH_CURRENT   A1 
#define RUDDER_POT      A2
#define WINCH_POT       A3
#define I2C_SDA         A4
#define I2C_SCL         A5

//Digital pins
#define RUDDER_PWM       3  //Motor Shield Channel A
#define GPS_RX           4
#define GPS_TX           5
#define WIND_VANE_TXD    6
#define RUDDER_RC        7
#define TELE_TX          8
#define TELE_RX          9 
#define WINCH_RC         10
#define WINCH_PWM        11 //Motor Shield Channel B
#define RUDDER_DIRECTION 12 //Motor Shield Channel A
#define WINCH_DIRECTION  13 //Motor Shield Channel B

//Output
#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
#define PMTK_SET_NMEA_OUTPUT_RMCGGA  "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
#define PMTK_SET_NMEA_OUTPUT_OFF     "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

//Update rates
#define PMTK_SET_NMEA_UPDATE_1HZ     "$PMTK220,1000*1F"
#define PMTK_SET_NMEA_UPDATE_5HZ     "$PMTK220,200*2C"
#define PMTK_SET_NMEA_UPDATE_10HZ    "$PMTK220,100*2F"

#define GPRMC_SAMPLE                 "$GPRMC,052420.600,A,3404.3640,N,11708.7536,W,0.38,341.13,120316,,,D*7C"
#define GPWPL_SAMPLE                 "$GPWPL,3404.3640,N,11708.7529,W,TEST*4B"
#define GPBWC_SAMPLE                 "$GPBWC,220516,5130.02,N,00046.34,W,213.8,T,218.0,M,0004.6,N,EGLM*21"
//#define GPBWC_SAMPLE                 "$GPBWC,081837,,,,,,T,,M,,N,*13"


SoftwareSerial gpsSerial(GPS_TX, GPS_RX);
StreamTalker gps("GP", &gpsSerial);
MPU9250AHRS ahrs;
SoftwareSerial teleSerial(TELE_TX, TELE_RX);
StreamTalker serial("UP", &teleSerial);
//CourseComputer courseComputer;
TX23UWindSensor windSensor(WIND_VANE_TXD);
RCChannel ch1(1, RUDDER_RC);
RCChannel ch2(2, WINCH_RC);
CustomServo rudder(1, RUDDER_PWM, RUDDER_DIRECTION, RUDDER_CURRENT);
CustomServo winch(2, WINCH_PWM, WINCH_DIRECTION, WINCH_CURRENT);

Bus bus;

char buffer[MAX_SENTENCE_LENGTH];

RMCSentence rmc;
WPLSentence wpl;
BWCSentence bwc;


void setup() 
{
  //start serial connection
  //Serial.begin(9600);
  teleSerial.begin(57600);
  teleSerial.setTimeout(100);

  gpsSerial.begin(9600);
  gpsSerial.setTimeout(100);
  
  gps.putSentence(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  gps.putSentence(PMTK_SET_NMEA_UPDATE_1HZ);
  
  gps.addFilter("$PMTK");

  serial.addFilter("$GP");
  serial.addFilter("$HC");
  serial.addFilter("$WI");
  //courseComputer.putSentence(GPWPL_SAMPLE);

  ahrs.init();
  
  bus.subscribe(&windSensor);
  bus.subscribe(&gps);
  bus.subscribe(&ahrs);
  //bus.subscribe(&courseComputer);
  bus.subscribe(&serial);
  bus.subscribe(&ch1);
  bus.subscribe(&ch2);
  bus.subscribe(&rudder);
  bus.subscribe(&winch);
}

void loop() 
{
  //unsigned long t = millis();
  
  bus.exchange();

  //unsigned long elapsed = millis() - t;
  //if (elapsed > 100) {
  //  teleSerial.print("t = "); 
  //  teleSerial.println(elapsed);
  //}
}
