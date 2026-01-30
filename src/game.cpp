//
// Created by Brouse on 26/01/2026.
//

#include <Arduino.h>

#include <config.h>
#include <tetrisGame.h>
#include <timer.h>

// Game startup
void onInit(byte* payload, unsigned int length);
TetrisGame game(mqttClient);

// Process controls
uint8_t askForRotation  = 0;
uint8_t sack            = 0;
auto askForMovement     = direction::NONE;

void processControls();
void onMove(byte* payload, unsigned int length);
void onButton(byte* payload, unsigned int length);
void onRotate(byte* payload, unsigned int length);

// Tick game
Timer timer1;
volatile uint8_t tick = 0;


ISR(TIMER1_COMPA_vect)
{
    tick = 1;
}

void setup()
{
    // Init
    Serial.begin(BAUD_RATE);

    mqttClient.setup(MQTT_HOST, MQTT_PORT);
    mqttClient.connect(MQTT_CLIENT_ID);

    mqttClient.subscribe("tetris/onInit",   onInit);
    mqttClient.subscribe("tetris/button",   onButton);
    mqttClient.subscribe("tetris/move",     onMove);
    mqttClient.subscribe("tetris/rotate",   onRotate);

    timer1.setup(MODE_CTC, SCALING1024);
    timer1.start(TICK_1024_1s / 2);
}

void loop()
{
    while (true)
    {
        processControls();
        if (tick)
        {
            game.tick();
            tick = 0;
        }
        mqttClient.loop();
    }
}

void onButton(byte* payload, unsigned int length)
{
    sack = 1;
}


void onInit(byte* payload, unsigned int length)
{
    game.init();
    game.start();
}

void onMove(byte* payload, unsigned int length)
{
    if (length != 1) return;

    const auto direction = static_cast<direction_t>(payload[0]);
    askForMovement = direction;
}

void onRotate(byte* payload, unsigned int length)
{
    askForRotation = 1;
}

//TODO: Send mqtt to Processing to update move
void processControls()
{
    // Process sack
    if (sack)
    {
        game.sack();
        sack = 0;
    }

    // Process rotation
    if (askForRotation)
    {
        game.rotate();
        askForRotation = 0;
    }

    // Process movement
    if (askForMovement != direction::NONE)
    {
        game.move(askForMovement);
        askForMovement = direction::NONE;
    }
}

