#ifndef EMAIL_SENDER_H
#define EMAIL_SENDER_H 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "email_recipient_list.h"
int send_email(char *topic, char *message, char *sender, struct recipient *recipients, char *username, char *password, char *smtp_ip_adress, char *smtp_port);
#endif