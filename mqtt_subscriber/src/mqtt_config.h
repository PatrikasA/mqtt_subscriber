#include <stdbool.h>

struct config{
    char* broker;
    int port;
    bool use_tls;
    char* cert_file;
    char* username;
    char* password;
};