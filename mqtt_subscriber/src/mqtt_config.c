#include "mqtt_config.h"

void init_config(struct config *cfg) {
        strcpy(cfg->broker, "localhost");
        strcpy(cfg-> port, "1883");
        cfg->use_tls = false;
	//strcpy(cfg->cert_file, "/tmp/ca.cert.pem");
	cfg->use_password = false;
	strcpy(cfg->password, "");
        strcpy(cfg->username, "");
}