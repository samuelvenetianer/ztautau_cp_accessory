CC=g++

CFLAGS=-c -g -Wall `root-config --cflags`

LDFLAGS=`root-config --glibs`

all: hadhad_psi

hadhad_psi: hadhad_psi.cc
	$(CC) $(CFLAGS) -o hadhad_psi.o hadhad_psi.cc
	$(CC) $(LDFLAGS) -o hadhad_psi hadhad_psi.o
