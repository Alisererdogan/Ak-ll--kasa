CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm

TARGETS = akilli_kasa rapor_deneyi

.PHONY: all clean run test rapor

all: $(TARGETS)

akilli_kasa: akilli_kasa.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

rapor_deneyi: rapor_deneyi.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

run: akilli_kasa
	./akilli_kasa

rapor: rapor_deneyi
	./rapor_deneyi

test: all
	@bash test_suite.sh

clean:
	rm -f $(TARGETS) *.o
