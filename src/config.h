//
// Created by Brouse on 25/01/2026.
//

#ifndef CONFIG_H
#define CONFIG_H

#include <Ethernet2.h>
#include <mqttClient.h>

#define BAUD_RATE       115200

#define MQTT_HOST       "tom.uib.es"
#define MQTT_PORT       1883
#define MQTT_CLIENT_ID  "tetris-game"

// Define the mac and ethernet configuration
uint8_t mac[] = {0x00, 0x00, 0x00, 0x05};
auto ip_address = IPAddress(192, 168, 132, 165);

EthernetConfig ethConfig(mac, ip_address);

static MqttClient mqttClient(ethConfig);

#endif //CONFIG_H
