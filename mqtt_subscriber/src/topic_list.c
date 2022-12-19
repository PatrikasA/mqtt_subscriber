#include "topic_list.h"

void add_topic(struct topic_node** head, struct topic_node* topic)
{
    struct topic_node* temp = *head;
    if (temp == NULL) {
        *head = topic;
        return; 
    }
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = topic;
}

void delete_topic_list(struct topic_node *list)
{
    struct topic_node *to_delete = list;
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