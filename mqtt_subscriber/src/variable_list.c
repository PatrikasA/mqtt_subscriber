#include "variable_list.h"

void add_variable(struct variable** head, struct variable* variable)
{
    struct variable* temp = *head;
    if (temp == NULL) {
        *head = variable;
        return; 
    }
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = variable;
}

void delete_variable_list(struct variable *list)
{
    struct variable *to_delete = list;
    while (list != NULL) {
        list = list->next;
        if(to_delete !=NULL){
            to_delete -> next = NULL;
            free(to_delete);
        }
        to_delete = NULL;
        to_delete = list;
    }
}