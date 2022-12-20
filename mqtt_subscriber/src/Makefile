BIN:=mqtt_subscriber
CC:=gcc
LDFLAGS:=-lmosquitto -lsqlite3 -ljson-c -lcurl
OBJ:=$(SRC:.c=.o)
SRC:=$(wildcard *.c)


.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ)
		$(CC) -g $(SRC) -o $@ $(LDFLAGS)

clean:
		$(RM) $(BIN) $(OBJ)
