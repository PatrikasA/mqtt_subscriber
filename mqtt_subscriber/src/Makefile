BIN:=mqtt_subscriber
CC:=gcc
LDFLAGS:= -lmosquitto -lsqlite3 -lssl -lcurl -luci -ljson-c
OBJ:=$(SRC:.c=.o)
SRC:=$(wildcard *.c)


.PHONY: all clean

all: $(BIN)

$(BIN): $(SRC)
		$(CC) -o $@ $^ $(LDFLAGS)

clean:
		$(RM) $(BIN) $(OBJ)
