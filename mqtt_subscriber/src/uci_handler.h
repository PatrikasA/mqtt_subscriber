#include <string.h>
#include <uci.h>
#include <syslog.h>
#include "topic_list.h"
#include "event_list.h" 
#include "email_recipient_list.h"

#ifndef UCI_HANDLER_H
#define UCI_HANDLER_H
int load_events(struct topic_node **topic_list);
#endif