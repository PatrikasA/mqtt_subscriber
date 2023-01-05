#include <mosquitto.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdbool.h>
#include "message_database.h"
#include "mqtt_config.h"
#include "topic_list.h"

#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H
int init_mosquitto(struct mosquitto **mosq, struct config *cfg, int *id, struct topic_node *topics);
void end_mosquitto(struct mosquitto **mosq);
#endif