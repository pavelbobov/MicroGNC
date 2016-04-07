all: build

build:
	g++ ../src/Sentences/*.cpp \
	    ../src/Instruments/CourseComputer.cpp \
	    ../src/Instruments/MessageBus.cpp \
	    tests.cpp \
	    -o tests -I ../../Time -lm -g 
