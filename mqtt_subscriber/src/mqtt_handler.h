#include <mosquitto.h>
#include <stdio.h>
#include <stdlib.h>
#include "message_database.h"

int init_mosquitto(struct mosquitto** mosq, int* id);
void end_mosquitto(struct mosquitto** mosq);