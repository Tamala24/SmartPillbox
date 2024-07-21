/**
 * ELSE PRO ERASMUS BIP PROJECT
 * Project: Smart Pillbox
 * Team: 7
 * Year: 2023/24
 *
 * @file smartPillbox.ino
 * @par
 * COPYRIGHT NOTICE: (c) 2024, Andrea Nieto Gil. This software is placed in the
 * public domain and may be used for any purpose. However, this notice must not
 * be changed or removed. No warranty is expressed or implied by the publication
 * or distribution of this source code.
 *
 * Version Control
 * Version: 1.0   (2024/07/19) Complete Prototype
 */

#include "Config.h"

#include <WiFi.h>
#ifdef SSL_ROOT_CA
  #include <WiFiClientSecure.h>
#endif

#include <PubSubClient.h>
#include <ESP32Servo.h>
#include <Arduino.h>
#include <RTClib.h>
#include <cstdint> 
#include <string>
#include <Wire.h>
#include <DHT.h>

#include "pitches.h" 
#include "buffer_int.h"
#include "prot_var_int.h"

// Struct to store pill information
struct Pill 
{
  String name;
  int hourMinutes; // Time in minutes from midnight
  int color = 0;
  int music;
};

// Variables to track button state, light state, and servo state
bool button_pressed = false;
bool light_on = true;
bool servo_opened = false;

// Circular buffer and variable for alarms
Buffer_Circ_Int_t alarms_to_activate;
Var_Prot_Int_t current_pill;

// Maximum number of pills the system can handle
const int MAX_PILLS = 10;

// Array to store pills
Pill pills[MAX_PILLS];
int numPills = 0;

// Real-Time Clock instance
RTC_DS3231 rtc;

// Servo motor instance
Servo myServo;

// DHT sensor instance
DHT dht(DHTPIN, DHTTYPE);

// Task handles for different tasks
TaskHandle_t CheckAlarm_Task_Handle, ActivateAlarm_Task_Handle, 
  HumTemp_Task_Handle, RGB_Task_Handle;

// Task function declarations
void CheckAlarm_Task(void * parameter);
void ActivateAlarm_Task(void * parameter);
void HumTemp_Task(void * parameter);
void RGB_Task(void * parameter);

/**
 * @brief Initial setup of the program. Configures core concepts, initializes 
 *        WiFi connection, MQTT connection, subscribes to topics, and calls on_setup
 */
void setup() 
{
#ifdef LOGGER_ENABLED
  // Initialize serial communications
  Serial.begin(BAUDS);
  Serial.setDebugOutput(true);
  delay(1000);
  Serial.println();
#endif

  // Connect to WiFi
  wifi_connect();

  // Connect to MQTT broker with a client ID
  mqtt_connect(deviceID);

  // Subscribe to relevant MQTT topics
  subscribeToTopics();

  // Initialize buzzer and RGB pins
  pinMode(BUZZER_PIN, OUTPUT); 
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  analogWrite(PIN_BLUE, 0);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_RED, 0);
  
  // Initialize the RTC
  if (!rtc.begin()) 
  {
    Serial.println("Could not find RTC");
    while (1);
  }
  if (rtc.lostPower()) 
  {
    Serial.println("RTC lost the power, configuring the time.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.adjust(DateTime(2024, 7, 16, 16, 6, 0)); // Adjust RTC to specific time

  // Configure button
  config_button();

  // Create "HumTemp_Task" for temperature and humidity readings
  xTaskCreate(
    HumTemp_Task,                 /* Task function. */
    "HumTemp_Task",               /* Name of task. */
    10000,                        /* Stack size of task */
    &alarms_to_activate,          /* Parameter of the task */
    1,                            /* Priority of the task */
    &HumTemp_Task_Handle);        /* Task handle */

  // Create "CheckAlarm_Task" to check alarms
  xTaskCreate(
    CheckAlarm_Task,              /* Task function. */
    "CheckAlarm_Task",            /* Name of task. */
    10000,                        /* Stack size of task */
    &alarms_to_activate,          /* Parameter of the task */
    1,                            /* Priority of the task */
    &CheckAlarm_Task_Handle);     /* Task handle */
    
  // Create "ActivateAlarm_Task" to activate alarms
  xTaskCreate(
    ActivateAlarm_Task,           /* Task function. */
    "ActivateAlarm_Task",         /* Name of task. */
    10000,                        /* Stack size of task */
    &alarms_to_activate,          /* Parameter of the task */
    1,                            /* Priority of the task */
    &ActivateAlarm_Task_Handle);  /* Task handle */

  // Create "RGB_Task" for RGB LED control
  xTaskCreate(
    RGB_Task,                     /* Task function. */
    "RGB_Task",                   /* Name of task. */
    10000,                        /* Stack size of task */
    &alarms_to_activate,          /* Parameter of the task */
    1,                            /* Priority of the task */
    &RGB_Task_Handle);            /* Task handle */
}

/**
 * @brief Main loop of the program. Manages repetitive tasks such as WiFi 
 *        connection, MQTT connection, and actions defined in on_loop
 */
void loop() 
{
  // Repetitive tasks
  wifi_loop(); // Manage WiFi connection
  mqtt_loop(); // Manage MQTT connection
  delay(100);  // Short delay
}

// Task to control RGB LEDs based on the current pill's color
void RGB_Task(void * parameter) 
{
  int pill_index;
  while(true) 
  {
    get_value(&pill_index, &current_pill);
    if (pill_index != -1) 
    {
      RGB(pills[pill_index].color);
    }
    delay(1000);
  }
}

// Task to check if it's time for an alarm
void CheckAlarm_Task(void * parameter) 
{
  while(true) 
  {
    int currentTime = obtainCurrentTimeInMinutes();
    for (uint32_t i = 0; i < numPills; i++) 
    {
      if (pills[i].hourMinutes == currentTime) 
      {
        // Add code here to sound the alarm and display the color
        put_item(i, &alarms_to_activate);
      }
    }
    delay(60000); // Check every minute
  }
}

// Task to read current temperature and humidity
void HumTemp_Task(void * parameter) 
{
  while(true) {
    getCurrentTemperatureAndHumidity();
    delay(60000); // Read every minute
  }  
}

// Task to activate alarms
void ActivateAlarm_Task(void * parameter) 
{
  uint32_t pill_index;
  while(true) 
  {
    while (alarms_to_activate.counter) 
    {
      button_pressed = false; 
      get_item(&pill_index, &alarms_to_activate);
      set_value(pill_index, &current_pill);
      String name = pills[pill_index].name;
      // Publish the name of the current pill to take
      sendMessageByTopic(TOPIC_CURRENT_ALARM, name);
      if (!servo_opened) 
      {
        servoOpen();
      }
      while (button_pressed == false) 
      {
        // Sound the alarm for the pill
        Serial.print("It's time to take the pill: ");
        Serial.print(pills[pill_index].name);
        Serial.print("! Color: ");
        Serial.print(pills[pill_index].color);
        Serial.print(", Music: ");
        Serial.println(pills[pill_index].music);
        playBuzzer(pills[pill_index].music);
        delay(1000);
      }
    }
    
    delay(1000); // Wait 1 second to close
    if (servo_opened) 
    {
      delay(10000); // Wait 10 seconds to close
      servoClose();
      servo_opened = false;
    }
  }
}

/*** End of file ****/