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

volatile sig_atomic_t daemonize = 1;

void sig_handler(int signo)
{
    signal(SIGINT, NULL);
    syslog(LOG_ALERT, "Received signal: %d\n", signo);
    daemonize = 0;
}

struct topic_node *topics;

int main(int argc, char* argv[])
{
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    open_database_file();
    create_database();
    int rc = 0;
    int id = 12;
    struct mosquitto* mosq;
    struct config* cfg = malloc(sizeof(struct config));
    struct topic_node *topics = NULL;
    init_config(cfg);
    get_options(cfg, argc, argv);
    rc = load_events(&topics);
    rc = init_mosquitto(&mosq, cfg, &id, topics);
    while (daemonize) {
    }
    end_mosquitto(&mosq);
    close_database_file();
}