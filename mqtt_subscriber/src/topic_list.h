#include <stdio.h>
#include <stdlib.h>

#ifndef TOPIC_LIST_H
#define TOPIC_LIST_H

struct topic_node{
    char topic[100];
    struct event_node *events;
    struct topic_node *next;
};

void add_topic(struct topic_node **head, struct topic_node *topic);
void delete_topic_list(struct topic_node *list);

#endif