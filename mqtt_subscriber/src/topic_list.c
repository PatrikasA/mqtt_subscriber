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

int add_event_to_topic(struct topic_node** head, struct event_node* event)
{
    struct topic_node *temp = *head;
    if (temp == NULL) {
	struct topic_node *topic = malloc(sizeof(struct topic_node));
	topic->next		 = NULL;
	topic->events		 = event;
	strcpy(topic->topic, event->topic);
	*head = topic;
	return 0;
    }
    while (temp->next != NULL) {
	if (strcmp(temp->topic, event->topic) == 0) {
		add_event(&(temp->events), event);
		return 0;
	}
	temp = temp->next;
    }
    return 1;
}

void delete_topic_list(struct topic_node *list)
{
    struct topic_node *to_delete = list;
    while (list != NULL) {
        list = list->next;
        if(to_delete !=NULL){
            to_delete -> next = NULL;
	    delete_event_list(to_delete->events);
	    free(to_delete);
	}
        to_delete = NULL;
        to_delete = list;
    }
}