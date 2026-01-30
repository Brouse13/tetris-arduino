//
// Created by Brouse on 26/01/2026.
//

#include <Arduino.h>

#include <config.h>
#include <types.h>

#define PIN_JOYSTICK_Y   A0
#define PIN_JOYSTICK_X   A1
#define PIN_JOYSTICK_BTN A2

rotation_t rotation;
uint8_t button;

void processMovement();
void processRotation();
void resetPieceRotation(byte* payload, unsigned int length);

void setup()
{
    Serial.begin(BAUD_RATE);
    pinMode(PIN_JOYSTICK_X, INPUT);
    pinMode(PIN_JOYSTICK_Y, INPUT);
    pinMode(PIN_JOYSTICK_BTN, INPUT);

    // When a new piece arrives or the game starts, the direction must be reseted
    mqttClient.setup(MQTT_HOST, MQTT_PORT);

    mqttClient.connect("joystick");
}

void loop()
{
    while (true)
    {
        const uint16_t buttonValue = digitalRead(PIN_JOYSTICK_BTN);

        processMovement();
        processRotation();

        if (buttonValue == LOW)
        {
            constexpr uint8_t data[1] = { 1 };
            mqttClient.publish("tetris/button", data, 1);
            Serial.println("pressed");
        }

        mqttClient.loop();
        delay(200);
    }
}

void processMovement()
{
    const uint16_t xValue = analogRead(PIN_JOYSTICK_X);
    const uint16_t yValue = analogRead(PIN_JOYSTICK_Y);
    auto direction = direction::NONE;

    if      (xValue < 400) direction = direction::LEFT;
    else if (xValue > 600) direction = direction::RIGHT;
    else if (yValue > 600) direction = direction::DOWN;

    if (direction == direction::NONE) return;

    const uint8_t data[1] = { static_cast<uint8_t>(direction) };
    mqttClient.publish("tetris/move", data, 1);
    Serial.println("move");
}

void processRotation()
{
    const uint16_t yValue = analogRead(PIN_JOYSTICK_Y);

    if (yValue > 400) return;;

    constexpr uint8_t data[1] = { 1 };
    mqttClient.publish("tetris/rotate", data, 1);
    Serial.println("rotate");
}