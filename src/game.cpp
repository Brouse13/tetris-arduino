//
// Created by Brouse on 26/01/2026.
//

#ifdef GAME

#include <Arduino.h>

#include <config.h>
#include <tetrisGame.h>
#include <timer.h>

// Game startup
void onStartUp(byte* payload, unsigned int length);
TetrisGame game(mqttClient);

// Process controls
uint8_t askForRotation  = 0;
auto askForMovement     = direction::NONE;

void processControls();
void onMove(byte* payload, unsigned int length);
void onRotate(byte* payload, unsigned int length);

// Tick game
Timer timer1;
uint8_t tick = 0;


ISR(TIMER1_COMPA_vect) { tick = 1; }

void setup()
{
    // Init
    Serial.begin(BAUD_RATE);

    mqttClient.setup(MQTT_HOST, MQTT_PORT);
    mqttClient.connect(MQTT_CLIENT_ID);

    mqttClient.subscribe("tetris/startup",  onStartUp);
    mqttClient.subscribe("tetris/move",     onMove);
    mqttClient.subscribe("tetris/rotate",   onRotate);
    game.init();

    timer1.setup(MODE_NORMAL, SCALING1024);
    timer1.start(TICK_1024_1s);
}

void loop()
{
    uint8_t time = 0;
    while (true)
    {
        if (tick)
        {
            processControls();
            game.tick();
            tick = 0;
        }

        time++;
        if (time == 100)
        {
            mqttClient.loop();
            time = 0;
        }
    }
}

void onStartUp(byte* payload, unsigned int length)
{
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

void processControls()
{
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


#endif // GAME

