#auto-generated file, do not edit!
SHELL = /bin/tcsh
CC = g++
CFLAGS = -Wall -O3 -g -I ../aha -I../abstraction -I../driver -I../shared -I../simulation -I../util

ifeq ($(findstring "Darwin", "$(shell uname -s)"), "Darwin")
ifeq ("$(OPENGL)", "STUB")
CFLAGS += -I../driver/STUB/ -I../driver/STUB/GL/ -DNO_OPENGL
else
CFLAGS += -DOS_MAC
CFLAGS += -I/System/Library/Frameworks/GLUT.framework/Versions/A/Headers/
CFLAGS += -I/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers/
endif
else # not darwin
ifeq ("$(OPENGL)", "STUB")
CFLAGS += -I../driver/STUB/ -I../driver/STUB/GL/ -DNO_OPENGL
else
CFLAGS += -I/usr/include/GL
endif
endif

ABSTRACTION_SRC = $(wildcard ../abstraction/*.cpp)
DRIVER_SRC = $(wildcard ../driver/*.cpp)
SHARED_SRC = $(wildcard ../shared/*.cpp)
SIMULATION_SRC = $(wildcard ../simulation/*.cpp)
UTIL_SRC = $(wildcard ../util/*.cpp)
AHASTAR_SRC = $(wildcard ../aha/*.cpp)
AHASTARTESTS_SRC = $(wildcard ../tests/aha/*.cpp)
HPASTAR_SRC = $(wildcard ../hpa/*.cpp)
HPASTARTESTS_SRC = $(wildcard ../tests/hpa/*.cpp)
