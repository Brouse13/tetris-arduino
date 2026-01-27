//
// Created by Brouse on 24/01/2026.
//

#include <mqttClient.h>
#include <Arduino.h>

MqttClient* MqttClient::_instance = nullptr;

void MqttClient::mqttCallback(char* topic, byte* payload, unsigned int length)
{
    if (!_instance) return;

    _instance->handleMessage(topic, payload, length);
}

void MqttClient::handleMessage(char* topic, byte* payload, unsigned int length)
{
    for (const auto &callback : _callbacks)
    {
        if (strcmp(topic, callback.topic) != 0) continue;
        callback.callback(payload, length);
    }
}

void MqttClient::setup(const char broker_host[], const uint16_t broker_port)
{
    _instance = this;

    Ethernet.begin(
        _ethernet_configuration.mac_address,
        _ethernet_configuration.ip_address,
        _ethernet_configuration.dns_server,
        _ethernet_configuration.gateway
    );

    Serial.println(broker_host);
    Serial.println(broker_port);

    _mqtt_client.setServer(broker_host, broker_port);
    _mqtt_client.setCallback(mqttCallback);
}

bool MqttClient::connect(const char client_id[])
{
    while (!_mqtt_client.connected())
    {
        Serial.println("Attempting MQTT connection ...");

        // Attempt to connect
        if (_mqtt_client.connect(client_id))
        {
            Serial.println("Connected");
            return true;
        }
    }
    return false;

}

void MqttClient::publish(const char* topic, const uint8_t* message, const uint16_t length)
{
    _mqtt_client.publish(topic, message, length);
}

void MqttClient::loop()
{
    _mqtt_client.loop();
}

void MqttClient::subscribe(const char* topic, const MqttCallback callback)
{
    if (strlen(topic) > MAX_TOPIC_LENGTH - 1) return;

    callback_entry_t entry;
    strncpy(entry.topic, topic, MAX_TOPIC_LENGTH);
    entry.topic[MAX_TOPIC_LENGTH - 1] = '\0';
    entry.callback = callback;

    _callbacks.push_back(entry);
}

void MqttClient::unsubscribe(const char* topic)
{
    _mqtt_client.unsubscribe(topic);
    // TODO mirar como quitar los elementos del callback
}