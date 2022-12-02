#include <mosquitto.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <syslog.h>

volatile __sig_atomic_t daemonize = 1;



void sig_handler(int signo)
{
    signal(SIGINT, NULL);
    syslog(LOG_ALERT, "Received signal: %d\n", signo);
    daemonize = 0;
}

void on_connect(struct mosquitto* mosq, void* obj, int rc)
{
    printf("ID: %d\n", *(int*) obj);
    if(rc){
        printf("Error with result code: %d",rc);
        exit(-1);
    }
    mosquitto_subscribe(mosq, NULL, "test/t1", 0);
}

void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg)
{
    printf("New message with topic: %s: %s\n", msg->topic, (char*) msg->payload);
}

int main(int argc, char* argv[])
{
    int rc=0;
    int id = 12;

    mosquitto_lib_init();
    struct mosquitto* mosq;
    mosq = mosquitto_new("subscribe-test", true, &id);
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    rc = mosquitto_connect(mosq, "localhost", 1883, 10);
    if(rc){
        printf("Could not connect to broker with RC=%d",rc);
    }

    mosquitto_loop_start(mosq);
    printf("Press enter to stop \n");
    getchar();
    mosquitto_loop_stop(mosq, true);
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}