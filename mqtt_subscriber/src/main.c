#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <syslog.h>
#include "mqtt_handler.h"
#include "message_database.h"


volatile __sig_atomic_t daemonize = 1;

void sig_handler(int signo)
{
    signal(SIGINT, NULL);
    syslog(LOG_ALERT, "Received signal: %d\n", signo);
    daemonize = 0;
}

int main(int argc, char* argv[])
{
    open_log_file();
    create_database();
    int rc=0;
    int id = 12;
    struct mosquitto* mosq;


    init_mosquitto(&mosq, &id);
    printf("Press enter to stop \n");
    getchar();
    end_mosquitto(&mosq);
    close_log_file();
}