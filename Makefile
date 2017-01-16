PLAT ?= linux
SHARED := -fPIC --shared
CFLAGS = -g -O2 -Wall

all: lvedis base64

lvedis : lua-vedis.c vedis.c
	$(CC) $(CFLAGS) $(SHARED) $^ -o lvedis.so

base64 : lua-base64.c
	$(CC) $(CFLAGS) $(SHARED) $^ -o base64.so

clean:
	rm *.so *.vedis
