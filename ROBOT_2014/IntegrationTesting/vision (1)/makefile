# Makefile for IEEE-Robotics vision software

SHELL=/bin/bash

# Set compilation options:

CC=gcc
CXX=g++
INSTALL_PATH = /usr/local
OBJDIR = obj
BINDIR = bin
SRCDIR = src
CXXFLAGS += -march=armv7-a -mtune=cortex-a8 -ffast-math -O3 -mfpu=neon -mfloat-abi=hard -Wall -W
CFLAGS += -std=c99  -march=armv7-a -mtune=cortex-a8 -ffast-math -O3 -mfpu=neon -mfloat-abi=hard -Wall -W
OBJECTS = $(OBJDIR)/main.o $(OBJDIR)/target.o $(OBJDIR)/servo.o $(OBJDIR)/gpio.o $(OBJDIR)/hardware.o

# Make targets

all: $(BINDIR)/main

debug: CXXFLAGS += -DDEBUG -g -ggdb3 -O0
debug: CFLAGS += -DDEBUG -g -ggdb3 -O0
debug: $(BINDIR)/main

install: $(BINDIR)/main
	mkdir -p $(INSTALL_PATH)/bin
	cp $(BINDIR)/main $(INSTALL_PATH)/bin/ieee_opencv
	cp $(SRCDIR)/startup.sh $(INSTALL_PATH)/bin/ieee_opencv_startup.sh
	chmod +x $(INSTALL_PATH)/bin/ieee_opencv_startup.sh
$(BINDIR)/main: $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags opencv` -o $@ $(OBJECTS) `pkg-config --libs opencv`
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(SRCDIR)/target.h $(SRCDIR)/servo.h $(SRCDIR)/gpio.h $(SRCDIR)/config.h
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRCDIR)/main.cpp
$(OBJDIR)/target.o: $(SRCDIR)/target.cpp $(SRCDIR)/target.h $(SRCDIR)/config.h
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRCDIR)/target.cpp
$(OBJDIR)/servo.o: $(SRCDIR)/servo.cpp $(SRCDIR)/servo.h $(SRCDIR)/config.h $(SRCDIR)/hardware.h
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRCDIR)/servo.cpp
$(OBJDIR)/gpio.o: $(SRCDIR)/gpio.cpp $(SRCDIR)/gpio.h $(SRCDIR)/config.h $(SRCDIR)/hardware.h
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRCDIR)/gpio.cpp
$(OBJDIR)/hardware.o: $(SRCDIR)/hardware.cpp $(SRCDIR)/hardware.h
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRCDIR)/hardware.cpp
clean:
	rm -rf $(OBJDIR) $(BINDIR) *.stackdump
