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
SoftwareSerial teleSerial(TELE_TX, TELE_RX);
StreamTalker serial("UP", &teleSerial);
CourseComputer courseComputer;
TX23UWindSensor windSensor(WIND_VANE_TXD);

Bus bus;

char buffer[NMEA_MAX_LENGTH];

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

  courseComputer.putSentence(GPWPL_SAMPLE);

  bus.subscribe(&windSensor);
  bus.subscribe(&gps);
  bus.subscribe(&courseComputer);
  bus.subscribe(&serial);

  pinMode(RUDDER_DIRECTION, OUTPUT);
  pinMode(WINCH_DIRECTION, OUTPUT);
  pinMode(RUDDER_PWM, OUTPUT);
  pinMode(WINCH_PWM, OUTPUT);
  
  pinMode(RUDDER_RC, INPUT);
  pinMode(WINCH_RC, INPUT);
}

void loop() 
{
  unsigned long t = millis();
  
  bus.exchange();

  //read the pushbutton value into a variable
  int ch1 = pulseIn(RUDDER_RC,25000) ;//- 1500;
  int ch2 = pulseIn(WINCH_RC,25000);// - 1500;
  //print out the value of the pushbutton
  //teleSerial.print(ch1);
  //teleSerial.print(" ");
  //teleSerial.println(ch2);

  if (ch1 == 0) {
    //No RC signal
    analogWrite(RUDDER_PWM, 0);
    analogWrite(WINCH_PWM, 0);
  } else {
    if (ch1 > 1510) {
      analogWrite(RUDDER_PWM, min((ch1 - 1500)/2, 255));
      digitalWrite(RUDDER_DIRECTION, HIGH);
    } else if (ch1 < 1490) {
      analogWrite(RUDDER_PWM, min((1500 - ch1)/2, 255));
      digitalWrite(RUDDER_DIRECTION, LOW);
    } else {
      analogWrite(RUDDER_PWM, 0);
    }
    
    if (ch2 > 1510) {
      analogWrite(WINCH_PWM, min((ch2 - 1500)/2, 255));
      digitalWrite(WINCH_DIRECTION, HIGH);
    } else if (ch2 < 1490) {
      analogWrite(WINCH_PWM, min((1500 - ch2)/2, 255));
      digitalWrite(WINCH_DIRECTION, LOW);
    } else {
      analogWrite(WINCH_PWM, 0);  
    }
  }

  delay(100);
  
  unsigned long elapsed = millis() - t;
  //Serial.print("t = "); Serial.println(elapsed);
}
