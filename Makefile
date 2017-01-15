PLAT ?= linux
SHARED := -fPIC --shared
CFLAGS = -g -O2 -Wall

all: lvedis

lvedis : lua-vedis.c vedis.c
	$(CC) $(CFLAGS) $(SHARED) $^ -o lvedis.so

clean:
	rm *.so *.vedis