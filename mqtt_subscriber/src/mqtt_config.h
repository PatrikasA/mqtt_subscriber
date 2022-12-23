#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H 1
#include <stdbool.h>
#include <string.h>


struct config{
    char broker[20];
    char port[10];
    bool use_tls;
    char cert_file[50];
    bool use_password;
    char username[20];
    char password[20];
};
void init_config(struct config *cfg);
#endif