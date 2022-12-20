#include "mqtt_handler.h"
#include "message_database.h"
#include "message_event.h"

extern struct topic_node* topics;

void exit_if_error(int rc, char* message)
{
    if(rc != MOSQ_ERR_SUCCESS)
    {
        syslog(LOG_ERR, "Error encountered. %s Exiting with code: %d", message, rc);
        mosquitto_lib_cleanup();
        exit(rc);
    }
}

static void on_connect(struct mosquitto* mosq, void* obj, int rc)
{
    exit_if_error(rc, "Failed to connect.");
}

static void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg)
{
    printf("New message with topic: %s: %s\n", msg->topic, (char*) msg->payload);
    //write_to_database(msg->topic, msg->payload);
    printf("%s", topics->topic);
    process_message(msg->topic, msg->payload, topics);
}

int subscribe_topic_list(struct mosquitto* mosq, struct topic_node* topics)
{
    int rc = 0;
    while (topics != NULL)
    {
        rc = mosquitto_subscribe(mosq, NULL, topics->topic, 1);
        if(rc != MOSQ_ERR_SUCCESS)
            syslog(LOG_ERR, "Failed to subscribe to topic \"%s\"\n", topics->topic);
        topics = topics->next;
    }

    return rc;
}

int init_mosquitto(struct mosquitto** mosq, struct config* cfg, int* id, struct topic_node* topics)
{
    int rc = 0;
    mosquitto_lib_init();

    if (cfg->use_tls == true)
    {
        rc = mosquitto_tls_set(*mosq, cfg->cert_file, NULL, NULL, NULL, NULL);
        exit_if_error(rc, "Failed to set TLS.");
    }
    if(cfg->use_password == true)
    {
        rc = mosquitto_username_pw_set(*mosq, cfg->username, cfg->password);
        exit_if_error(rc, "Failed to set username and password.");
    }

    *mosq = mosquitto_new("subscribe-test", true, id);
    mosquitto_connect_callback_set(*mosq, on_connect);
    mosquitto_message_callback_set(*mosq, on_message);
    rc = mosquitto_connect(*mosq, "localhost", 1883, 10);
    subscribe_topic_list(*mosq, topics);
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