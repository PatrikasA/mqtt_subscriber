#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef VARIABLE_LIST_H
#define VARIABLE_LIST_H
union data{
    double number;
    char string[100];
};

struct variable
{
    char key[256];
    bool is_number;
    union data data;
    struct variable *next;
};

void delete_variable_list(struct variable *list);
void add_variable(struct variable **head, struct variable *var);
#endif