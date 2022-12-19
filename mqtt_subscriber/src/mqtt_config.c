#include "mqtt_config.h"

void init_config(struct config *cfg) {
        cfg->broker = "asdasdasdasd";
        cfg-> port = "1883";
        cfg->use_tls = false;
        cfg->use_password = false;
        cfg->password = "";
        cfg->username = "";
}