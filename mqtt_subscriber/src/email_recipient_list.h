#include <stdio.h>
#include <stdlib.h>

#ifndef EMAIL_RECIPIENT_LIST_H
#define EMAIL_RECIPIENT_LIST_H

struct recipient{
    char email[100];
    struct recipient *next;
};
#endif