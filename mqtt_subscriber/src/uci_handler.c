#include "uci_handler.h"

int parse_string(char *expected, char *given, char *value, char *destination_address) {
        if (strcmp(expected, given) == 0) {
                strcpy(destination_address, value);
                return 0;
        }
        return -1;
}

int parse_int(char *expected, char *given, int value, struct event_node *e) {
        if (strcmp(expected, given) == 0) {
		e->operation = value;
		return 0;
        }
        return -1;
}

int event_parse_emails(struct event_node *event, struct uci_list *list, char *option) {
        if (strcmp("recipients", option) != 0) return -1;
        struct uci_element *i;
        struct recipient *recipient_list = NULL;
        uci_foreach_element(list, i) {
                struct recipient *r = malloc(sizeof(struct recipient));
                strcpy(r->email, i->name);
                add_recipient(&recipient_list, r);
        }
        event->recipients = recipient_list;
        return 0;
}

int event_parse_option(struct event_node *e, char *option, char *value) {
        if(parse_string("topic", option, value, e->topic)==0) return 0;
        if(parse_string("smtp_ip", option, value, e->smtp_ip)==0) return 0;
        if(parse_string("smtp_port", option, value, e->smpt_port)==0) return 0;
        if(parse_string("sender", option, value, e->sender)==0) return 0;
        if(parse_string("username", option, value, e->username)==0) return 0;
        if(parse_string("password", option, value, e->password)==0) return 0;
        if(parse_string("expected_value", option, value, e->expected_value)==0) return 0;
        if(parse_string("parameter", option, value, e->parameter)==0) return 0;
        if(parse_int("operation", option, atoi(value), e)==0) return 0;

        return -1;
}

int parse_section(struct uci_section *section, struct event_node *event) {
        struct uci_element *j;
        uci_foreach_element(&section->options, j) {
                struct uci_option *option = uci_to_option(j);
                char *option_name = option->e.name;
                if (option->type == UCI_TYPE_STRING) {
                        if (event_parse_option(event, option_name, option->v.string)) {
                                syslog(LOG_ERR,"ERROR: option %s could not be parsed\n",option->e.name);
                                return -1;
                        }
		} else {
			if (event_parse_emails(event, &option->v.list, option_name)) {
                                syslog(LOG_ERR, "ERROR: Failed to parse emails\n");
                                return -1;
                        }
		}
	}
        return 0;
}

int load_events(struct topic_node **topic_list) 
{
        struct uci_context *context = uci_alloc_context();
        struct uci_package *package;
        int rc = 0;

        if (uci_load(context, "mqtt_subscriber", &package) != UCI_OK) {
                uci_perror(context, "uci_load()");
                uci_free_context(context);
                return 1;
        }

        struct uci_element *i;
        uci_foreach_element(&package->sections, i) {
                struct uci_section *section = uci_to_section(i);
                char *section_type = section->type;
                char *section_name = section->e.name;
                struct event_node *event = malloc(sizeof(struct event_node));
                event->recipients = NULL;
		event->next		    = NULL;

		if (strcmp("event", section_type) == 0) {
                        rc = parse_section(section, event);
			if (!rc) {
				rc = add_event_to_topic(topic_list, event);
				if (rc) {
					delete_recipient_list(event->recipients);
                                        free(event);
				}
			} else {
				delete_recipient_list(event->recipients);
                                free(event);
			}
		} else {
                        free(event);
                }
        }
        uci_free_context(context);
        return 0;
}