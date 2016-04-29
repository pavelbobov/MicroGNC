/*
 * Geo.h
 * 
 * Geometric primitives and utilities.
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
 
#ifndef _GEO_H_
#define _GEO_H_

#define METERS_TO_NAUTICAL_MILES 0.000539957

/*
 * Latitude/Longitude point.
 */
class Point {
public:
  //Geographic coordinates in decimal degrees
  float latitude; // [-90, 90], > 0 Northern, < 0 Southern hemisphere
  float longitude; // [-180, 180], > 0 Eastern, < 0 Western hemisphere

  Point();
  Point(float lat, float lon);

  void set(float lat, float lon);
  void set(const Point& point);
};

/*
 * Great-circle arc from start to end point
 */
class Arc {
public:
  Point start;
  Point end;

  Arc();
  Arc(const Point& s, const Point& e);

  void set(const Point& s, const Point& e);

  //Great-circle initial bearing in degrees from start to end point
  float bearing() const;

  //Great-circle length of the arc in meters from start to end point (accuracy %0.5)
  float length() const;

  //Signed great-circle distance in meters from the given point to the arc,
  //a.k.a. cross-track error (XTE)
  float distance(const Point& p) const;
};

#endif /* _GEO_H_ */
