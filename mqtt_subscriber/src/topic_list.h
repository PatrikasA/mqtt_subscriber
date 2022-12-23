#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "event_list.h"

#ifndef TOPIC_LIST_H
#define TOPIC_LIST_H

struct topic_node{
    char topic[100];
    struct event_node *events;
    struct topic_node *next;
};

void add_topic(struct topic_node **head, struct topic_node *topic);
int add_event_to_topic(struct topic_node **head, struct event_node *event);
void delete_topic_list(struct topic_node *list);

#endif