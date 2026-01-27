//
// Created by Brouse on 25/01/2026.
//

#ifndef CONFIG_H
#define CONFIG_H

#include <Ethernet2.h>
#include <mqttClient.h>

#define BAUD_RATE       115200

#define MQTT_HOST       "192.168.1.174"
#define MQTT_PORT       1883
#define MQTT_CLIENT_ID  "tetris-game"

// Define the mac and ethernet configuration
uint8_t mac[] = {0x00, 0x00, 0x00, 0x12};
auto ip_address = IPAddress(192, 168, 1, 100);

EthernetConfig ethConfig(mac, ip_address);

static MqttClient mqttClient(ethConfig);

#endif //CONFIG_H
