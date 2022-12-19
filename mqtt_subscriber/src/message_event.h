#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <stdbool.h>
#include <syslog.h>

#include "event_list.h"
#include "variable_list.h"
#include "topic_list.h"

#ifndef MESSAGE_EVENT_H
#define MESSAGE_EVENT_H
int process_message(char *topic, char *message, struct topic_node *topic_list);
#endif