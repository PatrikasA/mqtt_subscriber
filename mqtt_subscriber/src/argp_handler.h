#ifndef ARGP_HANDLER_H
#define ARGP_HANDLER_H 1
#include <argp.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include "mqtt_config.h"
#include "message_database.h"

void get_options(struct config *cfg, int argc, char* argv[]);
#endif
