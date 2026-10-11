# Akilli Kasa Projesi Makefile
# Derleyici ve bayrak tanimlari
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -O2
LDFLAGS = -lm

# Hedef calistirilabilir dosyalar
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
	@chmod +x test_suite.sh
	@./test_suite.sh

clean:
	rm -f $(TARGETS) *.o
