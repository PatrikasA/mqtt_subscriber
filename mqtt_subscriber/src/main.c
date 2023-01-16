#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <syslog.h>
#include "mqtt_handler.h"
#include "message_database.h"
#include "argp_handler.h"
#include "mqtt_config.h"
#include "topic_list.h"
#include "event_list.h"
#include "uci_handler.h"
#include "email_sender.h"

volatile sig_atomic_t daemonize = 1;

void sig_handler(int signo)
{
    signal(SIGINT, NULL);
    syslog(LOG_ALERT, "Received signal: %d\n", signo);
    daemonize = 0;
}

struct topic_node *topics = NULL;

int main(int argc, char* argv[])
{
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    open_database_file();
    create_database();
    struct config cfg = { .broker	= "localhost",
			  .port		= "1883",
			  .use_tls	= false,
			  .use_password = false,
			  .password	= "",
			  .username	= "" };
    get_options(&cfg, argc, argv);
    int rc = 0;
    int id = 12;
    struct mosquitto* mosq;
    rc		 = load_events(&topics);
    if (rc)
	    goto cleanup_topics;
    rc = init_mosquitto(&mosq, &cfg, &id, topics);
    if (rc)
	    goto cleanup_mosquitto;
    mosquitto_loop_start(mosq);
    while (daemonize) {
    }
    cleanup_mosquitto:
    end_mosquitto(&mosq);
    cleanup_topics:
    delete_topic_list(topics);
    close_database_file();
}