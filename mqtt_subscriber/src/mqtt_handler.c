#include "mqtt_handler.h"
#include "message_database.h"
#include "message_event.h"

extern struct topic_node* topics;

static void on_connect(struct mosquitto* mosq, void* obj, int rc)
{
    if(rc){
	syslog(LOG_ERR, "Failed to connect to broker");
    }
}

static void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg)
{
    write_to_database(msg->topic, msg->payload);
    process_message(msg->topic, msg->payload, topics);
}

int subscribe_topic_list(struct mosquitto* mosq, struct topic_node* topics)
{
    int rc = 0;
    while (topics != NULL)
    {
	rc = mosquitto_subscribe(mosq, NULL, topics->topic, 1);
	if (rc != MOSQ_ERR_SUCCESS)
		syslog(LOG_ERR, "Failed to subscribe to topic \"%s\"\n", topics->topic);
    else syslog(LOG_INFO, "Subscribet to topic: \"%s\"\n", topics->topic);
    topics = topics->next;
    }
    return rc;
}

int init_mosquitto(struct mosquitto** mosq, struct config* cfg, int* id, struct topic_node* topics)
{
    int rc = 0;
    mosquitto_lib_init();
    *mosq = mosquitto_new("subscribe-test", true, id);

    if (cfg->use_tls == true)
    {
	rc = mosquitto_tls_set(*mosq, cfg->cert_file, NULL, NULL, NULL, NULL);
	if (rc) {
	    syslog(LOG_ERR, "Failed to set TLS.");
	    return rc;
	}
    }
    if(cfg->use_password == true)
    {
	rc = mosquitto_username_pw_set(*mosq, cfg->username, cfg->password);
	if (rc) {
	    syslog(LOG_ERR, "Failed to set username and password.");
	    return rc;
	}
    }

    mosquitto_connect_callback_set(*mosq, on_connect);
    mosquitto_message_callback_set(*mosq, on_message);
    rc = mosquitto_connect(*mosq, cfg->broker, atoi(cfg->port), 10);    
    if (rc) {
	    syslog(LOG_ERR, "Failed to subscribe to broker.");
	    printf("%d\n", rc);
	    return rc;
    }
    subscribe_topic_list(*mosq, topics);
    return rc;
}

void end_mosquitto(struct mosquitto** mosq)
{
    mosquitto_loop_stop(*mosq, true);
    mosquitto_disconnect(*mosq);
    mosquitto_destroy(*mosq);
    mosquitto_lib_cleanup();
}