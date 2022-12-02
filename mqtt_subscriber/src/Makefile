BIN:=mqtt_subscriber
CC:=gcc
LIBS:=-libmosquito -luci
OBJ:=$(SRC:.c=.o)
SRC:=$(wildcard *.c)


.PHONY all clean

all: $(BIN)

$(BIN): $(OBJ)
		$(CC) -o $@ $^ $(LDFLAGS)

clean:
		$(RM) $(BIN) $(OBJ)