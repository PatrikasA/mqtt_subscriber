BIN:=mqtt_subscriber
CC:=gcc
LDFLAGS:=-lmosquitto
OBJ:=$(SRC:.c=.o)
SRC:=$(wildcard *.c)


.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ)
		$(CC) $(SRC) -o $@ $(LDFLAGS)

clean:
		$(RM) $(BIN) $(OBJ)