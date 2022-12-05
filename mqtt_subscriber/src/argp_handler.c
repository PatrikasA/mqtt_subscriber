#include "argp_handler.h"
#include "mqtt_config.h"

static error_t parse_opt(int key, char* arg, struct argp_state* state)
{
    struct config* cfg = state -> input;
    switch(key){
        case 'h':
            cfg -> host = arg;
            break;
        case 'p':
            cfg -> port = arg;
            break;
        case 't':
            cfg -> use_tls = true;
            cfg -> cert_file = arg;
            break;
        case 'u':
            cfg -> username = arg;
            break;
        case 'P': 
            cfg -> password = arg;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp_option options[] = {
    {"host", 'h', "host", 0, "Host IP adress"},
    {"port", 'p', "port", 0, "Host port"},
    {"cert_file", 't', "cert_file", 0, "Certificate file filepath"},
    {"user", 'u', "user", 0, "Subscriber username"},
    {"password", 'P', "password", 0, "Subscriber password"},
    {"topic", 'T', "topic", 0, "Topic to subscribe to"},
    {0}
};

static struct argp argp = {options, parse_opt, "", ""};

