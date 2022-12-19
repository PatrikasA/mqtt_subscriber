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


volatile __sig_atomic_t daemonize = 1;

void sig_handler(int signo)
{
    signal(SIGINT, NULL);
    syslog(LOG_ALERT, "Received signal: %d\n", signo);
    daemonize = 0;
}

struct topic_node *topics;

int main(int argc, char* argv[])
{
    //open_database_file();
    //create_database();
    int rc = 0;
    int id = 12;
    struct mosquitto* mosq;
    struct config* cfg = NULL;
    // struct topic_node temptopics = {"test_topic", {"test_topic", "temp", LESS_THAN, "computer.test@tester.com",
    // {"patrikas.armalis@gmail.com", NULL}, 10, "patrikas", "password", "testip", "testport", NULL}, NULL};
    // topics = &temptopics;

    get_options(cfg, argc, argv);

    init_mosquitto(&mosq, cfg, &id, topics);
    printf("Press enter to stop \n");
    getchar();
    end_mosquitto(&mosq);
    //close_database_file();
}