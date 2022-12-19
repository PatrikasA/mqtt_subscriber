#include <argp.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include "mqtt_config.h"

#ifndef ARGP_HANDLER_H
#define ARGP_HANDLER_H
void get_options(struct config *cfg, int argc, char* argv[]);
#endif
