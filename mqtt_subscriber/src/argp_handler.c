#include "argp_handler.h"

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct config *cfg = state->input;
    switch (key)
    {
    case 'b':
	    strcpy(cfg->broker, arg);
	    break;
    case 'p':   
        strcpy(cfg->port, arg);
        break;
    case 't':
        cfg->use_tls = true;
        strcpy(cfg->cert_file, arg);
        break;
    case 'u':
        strcpy(cfg->username, arg);
        break;
    case 'P':
        strcpy(cfg->password, arg);
        cfg->use_password = true;
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp_option options[] = { { "broker", 'b', "broker", 0, "Broker IP adress" },
					{ "port", 'p', "port", 0, "Broker port" },
					{ "cert_file", 't', "cert_file", 0, "Certificate file filepath" },
					{ "user", 'u', "user", 0, "Subscriber username" },
					{ "password", 'P', "password", 0, "Subscriber password" },
					{ 0 } };

static struct argp argp = {options, parse_opt, "", ""};

static void exit_with_message(char *message, struct config *cfg)
{
    syslog(LOG_ERR, message);
    close_database_file();
    exit(1);
}

void argp_validate(struct config *cfg)
{
    if(atoi(cfg -> port) <=0 || atoi(cfg -> port) > 65535)
        exit_with_message("Port number out of bounds. Please enter a valid port.\n", cfg); 
    if(strlen(cfg -> broker) == 0)
        exit_with_message("Broker IP adress not provided\n", cfg);
    if(cfg -> use_tls == true)
        if(access(cfg -> cert_file, F_OK) != 0)
            exit_with_message("TLS enabled, no certificate file provided", cfg);
    if(strlen(cfg -> password) == 0 && strlen(cfg -> username) != 0)
        exit_with_message("Provided username, but no password. Both or neither must be provided", cfg);
    if(strlen(cfg -> username) == 0 && strlen(cfg -> password) != 0)
        exit_with_message("Provided password, but no username. Both or neither must be provided", cfg);
}

void get_options(struct config *cfg, int argc, char* argv[])
{
    argp_parse(&argp, argc, argv, 0, 0, cfg);
    argp_validate(cfg);
}