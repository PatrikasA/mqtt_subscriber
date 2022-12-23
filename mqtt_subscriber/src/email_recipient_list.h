#ifndef EMAIL_RECIPIENT_LIST_H
#define EMAIL_RECIPIENT_LIST_H 1
#include <stdio.h>
#include <stdlib.h>

struct recipient{
    char email[100];
    struct recipient *next;
};

void add_recipient(struct recipient **head, struct recipient *mail);
void delete_recipient_list(struct recipient *list);
#endif