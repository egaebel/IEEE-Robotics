# Makefile for IEEE-Robotics vision software

SHELL=/bin/bash

# Set compilation options:

CC=gcc
CXX=g++
INSTALL = /usr/local/bin/install -c
CXXFLAGS += -march=armv7-a -mtune=cortex-a8 -ffast-math -O3 -mfpu=neon -mfloat-abi=hard -Wall -W `pkg-config --cflags opencv`
CFLAGS += -std=c99  -march=armv7-a -mtune=cortex-a8 -ffast-math -O3 -mfpu=neon -mfloat-abi=hard -Wall -W `pkg-config --cflags opencv`
OBJECTS = main.o target.o servo.o gpio.o

# Make targets

all: main

debug: CXXFLAGS += -DDEBUG -g -ggdb3 -O0
debug: CFLAGS += -DDEBUG -g -ggdb3 -O0
debug: main

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o main $(OBJECTS) `pkg-config --libs opencv`
main.o: main.cpp target.o servo.o gpio.o config.h
	$(CXX) $(CXXFLAGS) -c main.cpp `pkg-config --libs opencv`
target.o: target.cpp target.h config.h
	$(CXX) $(CXXFLAGS) -c target.cpp `pkg-config --libs opencv`
servo.o: servo.cpp servo.h config.h hardware.o
	$(CXX) $(CXXFLAGS) -c servo.cpp `pkg-config --libs opencv`
gpio.o: gpio.cpp gpio.h config.h hardware.o
	$(CXX) $(CXXFLAGS) -c gpio.cpp `pkg-config --libs opencv`
hardware.o: hardware.cpp hardware.h
	$(CXX) $(CXXFLAGS) -c hardware.cpp `pkg-config --libs opencv`
clean:
	rm -f *.o *.stackdump
