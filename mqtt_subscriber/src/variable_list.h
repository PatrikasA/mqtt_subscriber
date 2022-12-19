#include <stdbool.h>
#include <stdlib.h>

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