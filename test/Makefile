# use normal make for this Makefile
#
# Makefile for building user programs to run on top of ManaOS
#
# Several things to be aware of:
#
#    ManaOS assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# if you are cross-compiling, you need to point to the right executables
# and change the flags to ld and the build procedure for as
#GCCDIR = ~/gnu/local/decstation-ultrix/bin/
GCCDIR = /home/ivo/Documents/facultad/ManaOS/test/mips-dec-ultrix42/mips-
LDFLAGS = -T src/script -N
ASFLAGS = -mips1
CPPFLAGS = $(INCDIR)

SRCS := $(wildcard src/*.c)
BINS := $(SRCS:src/%.c=bin/%)

# if you aren't cross-compiling:
#GCCDIR = ../test/mips-dec-ultrix42/mips-
#LDFLAGS = -N -T 0
#ASFLAGS =
#CPPFLAGS = -P $(INCDIR)


CC = $(GCCDIR)gcc
AS = $(GCCDIR)as
LD = $(GCCDIR)ld

CPP = /lib/cpp
INCDIR =-I../userprog -I../threads
CFLAGS = -G 0 -c $(INCDIR) -mips1

.PHONY: all
all: lib/gcc-lib build/start.o $(BINS)
	
lib/gcc-lib:
	ln -s /home/ivo/Documents/facultad/ManaOS/test/mips-dec-ultrix42/ lib/gcc-lib

build/start.o: src/start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) src/start.s > src/strt.s
	$(AS) $(ASFLAGS) -o build/start.o src/strt.s
	rm src/strt.s

bin/%: src/%.c 
	$(CC) $(CFLAGS) $< -o build/$*.o
	$(LD) $(LDFLAGS) build/start.o build/$*.o -o build/$*.coff
	../bin/coff2noff build/$*.coff $@
	

