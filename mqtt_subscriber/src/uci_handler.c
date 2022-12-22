#include "uci_handler.h"

int parse_string(char *expected, char *given, char *value, char *destination_address) {
        if (strcmp(expected, given) == 0) {
                strcpy(destination_address, value);
                return 0;
        }
        return -1;
}

int event_parse_emails(struct event_node *e, struct uci_list *list, char *option) {
        if (strcmp("recipients", option) != 0) return -1;
        struct uci_element *i;
        struct recipient *recipient_list = malloc(sizeof(struct recipient));
        uci_foreach_element(list, i) {
                struct recipient *m = malloc(sizeof(struct recipient));
                strcpy(m->email, i->name);
                add_recipient(&recipient_list, recipient);
        }
        return 0;
}

int event_parse_option(struct event_node *e, char *option, char *value) {
        if(parse_string("topic", option, value, e->topic)==0) return 0;
        if(parse_string("smtp_ip", option, value, e->smtp_url)==0) return 0;
        if(parse_string("smtp_port", option, value, e->variable_name)==0) return 0;
        if(parse_string("sender", option, value, e->sender_email)==0) return 0;
        if(parse_string("username", option, value, e->smtp_username)==0) return 0;
        if(parse_string("password", option, value, e->smtp_password)==0) return 0;
        if(parse_string("expected_value", option, value, e->exp_value)==0) return 0;
        if(parse_string("parameter", option, value, e->variable_name)==0) return 0;
        if(parse_email(option, value, e)==0) return 0;
        if(parse_variable_type(option, value, e)==0) return 0;
        if(parse_cmp_type(option, value, e)==0) return 0;
        return -1;
}

int load_events(struct topic **topic_list) {
        struct uci_context *context = uci_alloc_context();
        struct uci_package *package;
        int rc = 0;

        if (uci_load(context, CONFIG_NAME, &package) != UCI_OK) {
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
                event->receiver_address = NULL;

                if (strcmp("event", section_type) == 0) {
                        rc = parse_section(section, event);
                        if (rc == 0) {
                                rc = add_to_topic_list(topic_list, event);
                                if (rc != 0) {
                                        delete_recipient_list(event->receiver_address);
                                        free(event);
                                }
                        } else {
                                delete_recipient_list(event->receiver_address);
                                free(event);
                        }
                } else {
                        free(event);
                }
        }
        uci_free_context(context);
        return 0;
}

int parse_section(struct uci_section *section, struct event *event) {
        struct uci_element *j;
        uci_foreach_element(&section->options, j) {
                struct uci_option *option = uci_to_option(j);
                char *option_name = option->e.name;
                if (option->type == UCI_TYPE_STRING) {
                        if (event_parse_option(event, option_name, option->v.string) != 0) {
                                syslog(LOG_ERR,"ERROR: option could not be parsed\n");
                                return -1;
                        }
                } else {
                        if (event_parse_emails(event, &option->v.list, option_name) != 0) {
                                syslog(LOG_ERR, "ERROR: Failed to parse emails\n");
                                return -1;
                        }
                }
        }
        return 0;
}