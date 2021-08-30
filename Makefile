CC=gcc
CFLAGS=--std=gnu17 -Wall -Wextra -fstack-protector-all -pie -fPIE -D_FORTIFY_SOURCE=2
LDFLAGS=

ifeq ($(DEBUG),1)
	CFLAGS += -g -DDEBUG
else
	CFLAGS += -O3 -s
endif

all: eepparse

C_FILES := $(wildcard src/*.c)
H_FILES := $(wildcard src/*.h)
O_FILES := $(C_FILES:.c=.o)

eepparse: $(O_FILES)
	$(CC) $(CFLAGS) $(LDFLAGS) $(O_FILES) -o eepparse

%.o: %.c %.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -c $<

install: eepparse
	cp eepparse /usr/local/bin/

clean:
	rm -f eepparse $(O_FILES)
