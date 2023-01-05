#include "email_recipient_list.h"

void add_recipient(struct recipient** head, struct recipient* mail)
{
	mail->next	       = NULL;
	struct recipient *temp = *head;
	if (temp == NULL) {
		*head = mail;
		return; 
    }
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = mail;
}

void delete_recipient_list(struct recipient *list)
{
    struct recipient *to_delete = list;
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