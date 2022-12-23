#include "event_list.h"

void add_event(struct event_node** head, struct event_node* event)
{
    struct event_node* temp = *head;
    if (temp == NULL) {
        *head = event;
        return; 
    }
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = event;
}

void delete_event_list(struct event_node *list)
{
    struct event_node *to_delete = list;
    while (list != NULL) {
        list = list->next;
        if(to_delete !=NULL){
            to_delete -> next = NULL;
	    delete_recipient_list(to_delete->recipients);
	    free(to_delete);
	}
        to_delete = NULL;
        to_delete = list;
    }
}