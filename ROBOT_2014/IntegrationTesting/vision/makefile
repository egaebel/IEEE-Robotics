# Makefile for IEEE-Robotics vision software

SHELL=/bin/bash

# Set compilation options:

CC=gcc
CXX=g++
INSTALLDIR = /usr/local/bin/install -c
OBJDIR = obj
BINDIR = bin
SRCDIR = src
CXXFLAGS += -march=armv7-a -mtune=cortex-a8 -ffast-math -O3 -mfpu=neon -mfloat-abi=hard -Wall -W `pkg-config --cflags opencv`
CFLAGS += -std=c99  -march=armv7-a -mtune=cortex-a8 -ffast-math -O3 -mfpu=neon -mfloat-abi=hard -Wall -W `pkg-config --cflags opencv`
OBJECTS = $(OBJDIR)/main.o $(OBJDIR)/target.o $(OBJDIR)/servo.o $(OBJDIR)/gpio.o $(OBJDIR)/hardware.o

# Make targets

all: $(BINDIR)/main

debug: CXXFLAGS += -DDEBUG -g -ggdb3 -O0
debug: CFLAGS += -DDEBUG -g -ggdb3 -O0
debug: $(BINDIR)/main

$(BINDIR)/main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) `pkg-config --libs opencv`
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(SRCDIR)/target.h $(SRCDIR)/servo.h $(SRCDIR)/gpio.h $(SRCDIR)/config.h
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRCDIR)/main.cpp `pkg-config --libs opencv`
$(OBJDIR)/target.o: $(SRCDIR)/target.cpp $(SRCDIR)/target.h $(SRCDIR)/config.h
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRCDIR)/target.cpp `pkg-config --libs opencv`
$(OBJDIR)/servo.o: $(SRCDIR)/servo.cpp $(SRCDIR)/servo.h $(SRCDIR)/config.h $(SRCDIR)/hardware.h
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRCDIR)/servo.cpp `pkg-config --libs opencv`
$(OBJDIR)/gpio.o: $(SRCDIR)/gpio.cpp $(SRCDIR)/gpio.h $(SRCDIR)/config.h $(SRCDIR)/hardware.h
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRCDIR)/gpio.cpp `pkg-config --libs opencv`
$(OBJDIR)/hardware.o: $(SRCDIR)/hardware.cpp $(SRCDIR)/hardware.h
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRCDIR)/hardware.cpp `pkg-config --libs opencv`
clean:
	rm -rf $(OBJDIR) $(BINDIR) *.stackdump
