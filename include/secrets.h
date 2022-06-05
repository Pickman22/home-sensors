#ifndef SECRETS_H
#define SECRETS_H

#include "ESP8266WiFi.h"

typedef struct Secrets_tag {
  String ssid;
  String pass;
} Secrets;

extern Secrets secrets;

#endif // SECRETS_H
