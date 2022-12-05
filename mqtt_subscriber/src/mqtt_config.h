#include <stdbool.h>

struct config{
    char* host;
    int port;
    bool use_tls;
    char* cert_file;
    char* username;
    char* password;
};