/**
 * @file  e_mqtt_lib_no_tocar.ino
 * @brief File containing functions for MQTT connection management
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2024 Andrea Nieto Gil. All rights reserved.
 */

#define MQTT_CONNECTION_RETRIES 3  ///< Number of retries for MQTT connection

PubSubClient mqttClient(espWifiClient);  ///< MQTT client using the WiFi client

// MQTT CONFIG
const char* mqttServerIP = MQTT_SERVER_IP; ///< MQTT server IP address
unsigned int mqttServerPort = MQTT_SERVER_PORT; ///< MQTT server port
String mqttClientID; ///< MQTT client ID

/**
 * @brief Function to manage the MQTT connection in the loop
 */
void mqtt_loop() 
{
  // Reconnect to MQTT broker if disconnected and subscribe to topics
  if (!mqttClient.connected()) 
  {
    mqtt_reconnect(MQTT_CONNECTION_RETRIES);
    subscribeToTopics();
  }
  mqttClient.loop();  // Call the loop method to maintain the connection
}

/**
 * @brief Function to connect to the MQTT server
 * @param clientID MQTT client ID
 */
void mqtt_connect(String clientID) 
{
    // Configure MQTT client
    mqttClientID = String(clientID);
    mqttClient.setServer(mqttServerIP, mqttServerPort);

    // Set 'mqttCallback' as the function to be invoked upon receiving messages on subscribed topics
    mqttClient.setCallback(mqttCallback);

    // Connect to the MQTT server
    mqtt_reconnect(MQTT_CONNECTION_RETRIES);
}

/**
 * @brief Function to reconnect to the MQTT server if the connection is lost
 * @param retries Number of connection attempts
 */
void mqtt_reconnect(const uint32_t retries) 
{
  // Return if not connected to WiFi
  if (!WiFi.isConnected())
  {
    return;
  }

  if (!mqttClient.connected())
  {
    warnln("Disconnected from the MQTT broker");
  }

  // Attempt reconnection a specified number of times
  uint32_t r = 0;
  while (!mqttClient.connected() && r < retries) 
  {
    r++;

    trace("Attempting an MQTT connection to: 'mqtt://");
    trace(mqttServerIP);
    trace(":");
    trace(mqttServerPort);
    trace("' with client-id: '");
    trace(mqttClientID);
    traceln("' ... ");

    // Attempt to connect
    // boolean connect (clientID, [username, password], [willTopic, willQoS, willRetain, willMessage], [cleanSession])

    #ifdef MQTT_USERNAME
      if (mqttClient.connect(mqttClientID.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) 
      {
    #else
      if (mqttClient.connect(mqttClientID.c_str())) 
      {
    #endif
        debugln("-=- Connected to MQTT Broker");
        // Allow time for the connection to fully establish
        delay(1000);
      } 
      else 
      {
        debug("-X- failed, rc=");
        debugln(mqttClient.state());
        debugln("-R-   re-trying in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
  }
}

/**
 * @brief Callback function to process received MQTT messages
 * @param topic MQTT topic of the message
 * @param message Content of the MQTT message
 * @param length Length of the message
 */
void mqttCallback(char * topic, byte * message, unsigned int length) 
{
  // Function invoked automatically upon receiving messages on subscribed topics

  // Load received messages into a variable
  String incomingMessage;
  for (uint32_t i = 0; i < length; i++) 
  {
    incomingMessage += (char)message[i];
  }

  traceln("<<~~ RECEIVING an MQTT message:");
  traceln(topic);
  traceln(incomingMessage);

  whenReceivingMessageByTopic(topic, incomingMessage);  // Process the received message
}

/**
 * @brief Function to publish an MQTT message to a specific topic
 * @param topic MQTT topic where the message will be published
 * @param outgoingMessage Content of the message to publish
 */
void mqtt_publish(const char * topic, String outgoingMessage) 
{
  if (!mqttClient.connected()) 
  {
    errorln("Cannot send message through the topic ... the MQTT Client is disconnected!!");
    return;
  }

  traceln("~~>> PUBLISHING an MQTT message:");
  traceln(topic);
  traceln(outgoingMessage);
  mqttClient.publish(topic, outgoingMessage.c_str());  // Publish the message
}

/**
 * @brief Function to subscribe to an MQTT topic
 * @param topic MQTT topic to subscribe to
 */
void mqtt_subscribe(const char * topic) 
{
  if (!mqttClient.connected()) 
  {
    errorln("Cannot subscribe to topic ... the MQTT Client is disconnected!!");
    return;
  }

  trace("Subscribed to topic: ");
  traceln(topic);
  mqttClient.subscribe(topic);  // Subscribe to the topic
}

/*** End of file ****/