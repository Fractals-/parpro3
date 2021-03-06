CXX = mpic++
CFLAGS = -Wall -Wextra -O3 -g -mcmodel=medium

all:	parsort

parsort:	parsort.o heapsort.o
	$(CXX) $(CFLAGS) -o $@ $^ #-lrt

%.o:	%.cc
	$(CXX) $(CFLAGS) -c $<

clean:
	rm -f *o
	rm -f parsort
