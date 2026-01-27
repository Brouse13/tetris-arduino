//
// Created by Brouse on 24/01/2026.
//

#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <Ethernet2.h>
#include <PubSubClient.h>

#include <vector.h>

#define MAX_TOPIC_LENGTH 20

typedef void (*MqttCallback)(byte* payload, unsigned int length);
typedef struct callback_entry
{
    char topic[MAX_TOPIC_LENGTH];
    MqttCallback callback;
} callback_entry_t;

class EthernetConfig
{
public:
    explicit EthernetConfig(uint8_t mac[4], const IPAddress& ip_address) :
        mac_address(mac),
        ip_address(ip_address)
    { }

    uint8_t* mac_address;

    const IPAddress ip_address;
    const IPAddress gateway = IPAddress(192, 168, 1, 1);

    const IPAddress dns_server = IPAddress(8, 8, 8, 8);
};

class MqttClient
{
public:
    /**
     * Create a new instance of MqttClient class.
     *
     * @param ethernet_configuration  ethernet configuration.
     */
    explicit MqttClient(const EthernetConfig& ethernet_configuration) :
        _ethernet_configuration(ethernet_configuration),
        _mqtt_client(_ethernet_client)
    { }


    /**
     * Create a new MQTT client configuration specifying the ethernet configuration.
     *
     * @param broker_host Broker host
     * @param broker_port Broker port
     */
    void setup(const char broker_host[], uint16_t broker_port);

    /**
     * Begin the connection to the broker using the provided configuration
     *
     * @param client_id Client id
     * @return if the client has init successfully
     */
    bool connect(const char client_id[]);

    /**
     * Subscribe to a new topic on the broker.
     *
     * @param topic     Topic to subscribe
     * @param callback  Callback to execute on topic match
     */
    void subscribe(const char* topic, MqttCallback callback);

    /**
    * Unsubscribe to the topic on the broker.
    *
    * @param topic Topic to unsubscribe
    */
    void unsubscribe(const char* topic);

    /**
     * Publish a new message on the broker
     *
     * @param topic Topic to publish
     * @param message message pointer to publish
     * @param length length of the message
     */
    void publish(const char* topic, const uint8_t* message, uint16_t length);

    /**
     * Loop the broker connection to avoid idle.
     */
    void loop();

private:
    static MqttClient*                  _instance;
    EthernetConfig                      _ethernet_configuration;
    EthernetClient                      _ethernet_client;
    PubSubClient                        _mqtt_client;
    StaticVector<callback_entry_t, 10>  _callbacks;

    static void mqttCallback(char* topic, byte* payload, unsigned int length);

    void handleMessage(char* topic, byte* payload, unsigned int length);
};

#endif //MQTTCLIENT_H