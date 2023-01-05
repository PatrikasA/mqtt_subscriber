#include "message_event.h"
#include "message_database.h"
#include "email_sender.h"
#include "email_recipient_list.h"

static int check_string(char* message, enum operation operation, char* compare_to)
{
    switch (operation)
    {
    case EQUAL:
        return (strcmp(compare_to, message) == 0);
        break;
    case NOT_EQUAL:
        return (strcmp(compare_to, message) != 0);
        break;
    case GREATER_THAN:
        return (strcmp(compare_to, message) > 0);
        break;
    case LESS_THAN:
        return (strcmp(compare_to, message) < 0);
        break;
    case LESS_THAN_EQUAL:
        return (strcmp(compare_to, message) <= 0);
        break;
    case GREATER_EQUAL:
        return (strcmp(compare_to, message) >= 0);
    default:
        return 2;
    };
}

static int check_number(double message, enum operation operation, double compare_to)
{
    switch (operation)
    {
    case EQUAL:
        return (message == compare_to);
        break;
    case NOT_EQUAL:
        return (compare_to != message);
        break;
    case GREATER_THAN:
        return (message > compare_to);
        break;
    case LESS_THAN:
        return (message < compare_to);
        break;
    case LESS_THAN_EQUAL:
        return (message <= compare_to);
        break;
    case GREATER_EQUAL:
        return (message >= compare_to);
    default:
        return 2;
    };
}

int parse_message(struct variable *var, char *message)
{

    for (int i = 0; i < strlen(message); i++)
    {

        if(!isdigit(message[i]))
        {
            var->is_number = false;
            strcpy(var->data.string, message);
            return 0;
        }
    }
    var->is_number = true;
    var->data.number = atof(message);
}

void execute_events(struct topic_node* current, struct variable* var)
{
    char value[100];
    struct event_node *current_event = current->events;
    int rc			     = 0;
    while (current_event != NULL) {
	if(var->is_number==true){
            rc = check_number(var->data.number, current_event->operation, atoi(current_event->expected_value));
	sprintf(value, "%f", var->data.number);
    }
	else {
        rc = check_string(var->data.string, current_event->operation, current_event->expected_value);
    strcpy(value, var->data.string);
    }
	if (rc == 1) {
    char message[1000];
    sprintf(message,
	    "Subject: %s\n\n Message from topic: %s \n Received value: %s \n Expected value: %s ", "MQTT event",
	    current_event->topic, value, current_event->expected_value);
    send_email(message, current_event->sender, current_event->recipients, current_event->username,
	       current_event->password, current_event->smtp_ip, current_event->smpt_port);
	}
	current_event = current_event->next;
    }
}

int process_message(char* topic, char* message, struct topic_node* topic_list)
{
    struct variable *var = malloc(sizeof(struct variable));
    parse_message(var, message);
    struct topic_node *current = topic_list;
    while (current != NULL)
    {
        if(strcmp(topic, current->topic) == 0){
            execute_events(current, var);
            free(var);
            return 0;
        }
        current = current->next;
    }
    free(var);
    return 0;
}
