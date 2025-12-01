CC=g++

CFLAGS=-c -g -Wall `root-config --cflags`

LDFLAGS=`root-config --glibs`

all: select_events

select_events: select_events.cc
	$(CC) $(CFLAGS) -o select_events.o select_events.cc
	$(CC) $(LDFLAGS) -o select_events select_events.o
