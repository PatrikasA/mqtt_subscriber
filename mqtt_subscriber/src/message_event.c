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

int parse_message(struct variable **var, char *message)
{
    json_object *object = json_tokener_parse(message);
    if(object == NULL){
	syslog(LOG_ERR, "Error: unsupported message formatting. Supported formatting: JSON");
	return 1;
    }
    enum json_type type;
    json_object_object_foreach(object, key, val)
    {
	struct variable *current = malloc(sizeof(struct variable));
	current->next		 = NULL;
	strcpy(current->key, key);
	type = json_object_get_type(val);
	switch (type) {
	case json_type_double:
		current->is_number   = true;
		current->data.number = json_object_get_double(val);
		break;
	case json_type_int:
		current->is_number   = true;
		current->data.number = json_object_get_int(val);
		break;
    case json_type_string:
		current->is_number = false;
        strcpy(current->data.string, json_object_get_string(val));
	    break;
    default:
	    syslog(LOG_ERR, "Error: encountered unsupported json type");
	    break;
    }
    add_variable(var, current);
    }
    json_object_put(object);
    return 0;
}

void execute_events(struct topic_node* current, struct variable* var)
{
    char value[100];
    struct event_node *current_event = current->events;
    int rc = 0;
    while (current_event != NULL) {
    struct variable *current_variable = var;
    while (current_variable != NULL) {
	    if (strcmp(current_variable->key, current_event->parameter)==0){
	    if (current_variable->is_number == true) {
		    rc = check_number(current_variable->data.number, current_event->operation,
				      atoi(current_event->expected_value));
		    sprintf(value, "%f", current_variable->data.number);
	    } else {
		    rc = check_string(current_variable->data.string, current_event->operation,
				      current_event->expected_value);
		    strcpy(value, current_variable->data.string);}
    char message[1000];
    sprintf(message,
	    "Subject: %s\n\n Message from topic: %s \n Parameter: %s \n Received value: %s \n Expected value: %s ", "MQTT event",
	    current_event->topic, current_variable->key, value, current_event->expected_value);
    send_email(message, current_event->sender, current_event->recipients, current_event->username,
	       current_event->password, current_event->smtp_ip, current_event->smpt_port);
	    }
    current_variable = current_variable->next;
	}
	current_event = current_event->next;
    }
}

int process_message(char* topic, char* message, struct topic_node* topic_list)
{
    struct variable *var=NULL;
    parse_message(&var, message);
    struct topic_node *current = topic_list;
    while (current != NULL) {
	if(strcmp(topic, current->topic) == 0){
    execute_events(current, var);
    delete_variable_list(var);
    return 0;
	}
        current = current->next;
    }
    free(var);
    return 0;
}
