#include "mqtt_handler.h"

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
    write_to_database(msg -> topic, msg ->payload);
}

int init_mosquitto(struct mosquitto** mosq, int* id)
{
    int rc = 0;
    mosquitto_lib_init();
    *mosq = mosquitto_new("subscribe-test", true, id);
    mosquitto_connect_callback_set(*mosq, on_connect);
    mosquitto_message_callback_set(*mosq, on_message);
    rc = mosquitto_connect(*mosq, "localhost", 1883, 10);
    mosquitto_loop_start(*mosq);
    return rc;
}

void end_mosquitto(struct mosquitto** mosq)
{
    mosquitto_loop_stop(*mosq, true);
    mosquitto_disconnect(*mosq);
    mosquitto_destroy(*mosq);
    mosquitto_lib_cleanup();
}