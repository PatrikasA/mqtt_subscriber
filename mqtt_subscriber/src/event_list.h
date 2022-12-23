#ifndef EVENT_LIST_H
#define EVENT_LIST_H 1
#include <stdio.h>
#include <stdlib.h>
#include "email_recipient_list.h"
enum operation
{
    LESS_THAN,
    GREATER_THAN,
    EQUAL,
    NOT_EQUAL,
    GREATER_EQUAL,
    LESS_THAN_EQUAL
};

struct event_node{
    char topic[50];
    char parameter[10];
    enum operation operation;
    char sender[40];
    struct recipient* recipients;
    char expected_value[200];
    char username[50];
    char password[50];
    char smtp_ip[20];
    char smpt_port[20];
    struct event_node *next;
};



void add_event(struct event_node **head, struct event_node *event);
void delete_event_list(struct event_node *list);
#endif