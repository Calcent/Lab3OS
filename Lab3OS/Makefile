CC      = gcc
CFLAGS  = -Wall -g
TARGETS = sjf rr

.PHONY: all clean

all: $(TARGETS)

sjf: SJF_inital.c
	$(CC) $(CFLAGS) -o $@ $<

rr: RR_inital.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)