CC=g++

CFLAGS=-c -g -Wall `root-config --cflags`

LDFLAGS=`root-config --glibs`

all: select_events_reco_only

select_events_reco_only: select_events_reco_only.cc
	$(CC) $(CFLAGS) -o select_events_reco_only.o select_events_reco_only.cc
	$(CC) $(LDFLAGS) -o select_events_reco_only select_events_reco_only.o
