all: build

build:
	g++ ../src/Sentences/*.cpp \
	    ../src/Instruments/CourseComputer.cpp \
	    ../src/Instruments/MessageBus.cpp \
	    tests.cpp \
	    -o tests -I /usr/local/share/arduino/libraries/Time -lm -g 
