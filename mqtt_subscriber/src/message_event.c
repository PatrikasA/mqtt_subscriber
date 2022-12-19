#include "message_event.h"
#include "event_list.h"
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

int parse_json_obj(struct variable **var, char *message)
{
    json_object *object = json_tokener_parse(message);
    enum json_type type;

    json_object_object_foreach(object, key, val)
    {
        struct variable *current;
        if((*var)->key == NULL)
            current = *var;
        else{
            current = malloc(sizeof(struct variable));
            current->next = *var;
            *var = current;
        }

        strcpy(current->key, key);
        type = json_object_get_type(object);

        switch(type){
            case json_type_null:
                syslog(LOG_ERR, "Encountered NULL json\n");
                return 1;
                break;
            case json_type_double:
                current->is_number = true;
                current->data.number = json_object_get_double(object);
                break;
            case json_type_int:
                current->is_number = true;
                current->data.number = json_object_get_int(object);
                break;
            case json_type_string:
                current->is_number = false;
                strcpy(current->data.string, json_object_get_string(object));
                break;
            default:
                syslog(LOG_ERR, "Unsupported json encountered\n");
                return 1;
                break;
            }
    }
    json_object_put(object);
    return 0;
}


void execute_events(struct topic_node* current, struct variable* var)
{
    struct event_node *current_event = current->events;
    struct variable *current_variable = var;
    int rc = 0;

    while (current_variable != NULL)
    {
        while(current_event != NULL)
        {
           if(current_variable->is_number==true)
               rc = check_number(current_variable->data.number, current_event->operation, atoi(current_event->expected_value));
            else
               rc = check_string(current_variable->data.string, current_event->operation, current_event->expected_value);

            if(rc == 1)
               send_email("topc", "sending an email event happened",
                          current_event->sender, current_event->recipients, current_event->username,
                          current_event->password, current_event->smtp_ip, current_event->smpt_port);

            current_event = current_event->next;
        }
        current_variable = current_variable->next;
    }
}

int process_message(char* topic, char* message, struct topic_node* topic_list)
{
    struct variable *var = malloc(sizeof(struct variable));
    if (var = NULL)
    {
        //Didnt malloc
    }

    if(parse_json_obj(&var, message) != 0){
        delete_variable_list(var);
        return 1;
    }

    struct topic_node *current = topic_list;
    while(current != NULL)
    {
        if(strcmp(topic, current->topic) == 0){
            execute_events(current, var);
        }
        current = current->next;
    }
}
