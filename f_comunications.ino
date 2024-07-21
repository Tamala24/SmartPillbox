/**
 * @file  g_comunications.ino
 * @brief Implementation of functions for managing MQTT communications
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2024 Andrea Nieto Gil. All rights reserved.
 */

/**
 * @brief Subscribes to MQTT topics to receive messages
 * @details This function sets up subscriptions to the MQTT topics for new pill data, removal of pills, and updates.
 */
void subscribeToTopics() 
{
  mqtt_subscribe(TOPIC_NEW_PILL); // Subscribe to the topic for new pill information
  mqtt_subscribe(TOPIC_RM);       // Subscribe to the topic for removing pills
  mqtt_subscribe(TOPIC_UPDATE);   // Subscribe to the topic for updating pills
}

/**
 * @brief Handles the reception of messages from MQTT topics
 * @param topic The MQTT topic from which the message was received
 * @param incomingMessage The message received on the topic
 * @details This function processes incoming messages based on their topic.
 *          It delegates message processing to appropriate functions based on the topic.
 */
void whenReceivingMessageByTopic(char * topic, String incomingMessage)
{
  // Check which topic the message is received from and handle accordingly
  if(strcmp(topic, TOPIC_NEW_PILL) == 0 )
  {
    processInput(incomingMessage); // Process the message related to new pills
  }
  else if(strcmp(topic, TOPIC_RM) == 0 )
  {
    removePill(incomingMessage); // Process the message related to removing pills
  }
  else if(strcmp(topic, TOPIC_UPDATE) == 0 )
  {
    updatePill(incomingMessage); // Process the message related to updating pills
  }
}

/**
 * @brief Sends a message to a specified MQTT topic
 * @param topic The MQTT topic where the message will be published
 * @param outgoingMessage The message to be published
 * @details This function publishes the provided message to the specified MQTT topic.
 */
void sendMessageByTopic(const char * topic, String outgoingMessage) 
{
  mqtt_publish(topic, outgoingMessage.c_str()); // Publish the message to the given topic
}

/*** End of file ****/