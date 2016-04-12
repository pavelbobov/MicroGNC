# MicroGNC [![Build Status](https://travis-ci.org/pavelbobov/MicroGNC.svg?branch=master)](https://travis-ci.org/pavelbobov/MicroGNC)

Guidance, Navigation and Control Library for Arduino

Autopilot components for model boats, cars, planes, rockets, and everything else that moves, can be steered, and has a mission. This library is intended to be light, easy to use, and extensible. It brings big industry standards and software development practices to small-scale projects.     

The library currently includes:
* Methods for computing bearings, distances, and cross-track errors on great-circle.  
* Classes for parsing and construction of some NMEA 0183 and proprietary sentences. (NMEA 0183 is a proprietary protocol issued by the National Marine Electronics Association for use in boat navigation and control systems.)
* Base class for instruments, such as sensors, sensor filter, controllers, and actuators.  
* StreamTalker class for reading/writing sentences from/to serial interface.
* CourseComputer class for computing direct course from current location to a waypoint.
* Message bus for exchanging messages among instruments. (Conceptually similar to NMEA 'backbone'.)
 