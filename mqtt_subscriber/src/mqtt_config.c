#include "mqtt_config.h"

void init_config(struct config *cfg) {
        strcpy(cfg->broker, "192.168.10.103");
        strcpy(cfg-> port, "8883");
        cfg->use_tls = true;
	strcpy(cfg->cert_file, "/tmp/ca.cert.pem");
	cfg->use_password = false;
	strcpy(cfg->password, "");
        strcpy(cfg->username, "");
}