/**
 * @file  g_button_interruption.ino
 * @brief Implementation of button interruption functions
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2024 Andrea Nieto Gil. All rights reserved.
 */

/**
 * @brief Interrupt Service Routine (ISR)
 * @details This function is called when a button press is detected.
 *          It sets the flag indicating that the button has been pressed,
 *          turns off the light, and prints a message to the serial monitor.
 */
void IRAM_ATTR isr() 
{
  button_pressed = true; // The button has been pressed
  light_on = false; // Turn off the light
  Serial.println("Button pressed!!"); // Print a message indicating button press
}

/**
 * @brief Configures the button interruption
 * @details This function sets up the button pin as an input with an internal pull-up resistor.
 *          It also assigns the 'isr' function to handle interruptions triggered by a falling edge.
 */
void config_button()
{
  pinMode(PIN_BUTTON, INPUT_PULLUP); // Set the button pin as input with pull-up resistor
  attachInterrupt(PIN_BUTTON, isr, FALLING); // Attach the 'isr' function to handle falling edge interrupts
}

/*** End of file ****/