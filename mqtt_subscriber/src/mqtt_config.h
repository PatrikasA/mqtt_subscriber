#include <stdbool.h>

#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

struct config{
    char *broker;
    char* port;
    bool use_tls;
    char* cert_file;
    bool use_password;
    char *username;
    char* password;
};

#endif