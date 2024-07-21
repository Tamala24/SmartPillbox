/**
 * @file  Config.h
 * @brief System configuration file
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2024 Andrea Nieto Gil. All rights reserved.
 */

#ifndef CONFIG_H
#define CONFIG_H

// Serial communication baud rate
#define BAUDS 115200

// Enable logger; comment out to disable serial console logging
#define LOGGER_ENABLED            

// Log level for c_logger: TRACE, DEBUG, INFO, WARN, ERROR, FATAL, NONE
#define LOG_LEVEL TRACE           

// WiFi configuration
#define NET_SSID                    "MyWifi"
#define NET_PASSWD                  "MyPassword"

// MQTT configuration
#define MQTT_SERVER_IP              "broker.emqx.io"
#define MQTT_SERVER_PORT            1883
#define MQTT_USERNAME               "emqx"    // Uncomment this line (and the next) to connect to the MQTT broker using username and password
#define MQTT_PASSWORD               "public"

// MQTT topics
#define TOPIC_TEMP_HUM              "team7/app/th"
#define TOPIC_NEW_PILL              "team7/app/pill"
#define TOPIC_RM                    "team7/app/rmpill"
#define TOPIC_UPDATE                "team7/app/uppill"
#define TOPIC_CURRENT_ALARM         "team7/app/currentAlarm"

// Pin configuration
// RGB LED pins
#define PIN_RED    25
#define PIN_GREEN  16
#define PIN_BLUE   17

// Buzzer pin
#define BUZZER_PIN 12 

// Servo motor pin for PWM output
#define SERVO_PIN 27                 

// DHT sensor pin and type
#define DHTPIN 14 
#define DHTTYPE DHT11

// Button pin
#define PIN_BUTTON 13

#endif // CONFIG_H

/*** End of file ****/