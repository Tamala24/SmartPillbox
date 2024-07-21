/**
 * @file  h_functions.ino
 * @brief Implementation of functions
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2024 Andrea Nieto Gil. All rights reserved.
 */

/**
 * @brief Converts a time string in "HH:MM" format to minutes since midnight.
 * 
 * @param time A String representing the time in "HH:MM" format.
 * @return int The number of minutes since midnight.
 */
int convertTimeToMinutes(String time) 
{
  int pos = time.indexOf(':'); // Find the position of the colon
  int hours = time.substring(0, pos).toInt(); // Extract hours part and convert to integer
  int minutes = time.substring(pos + 1).toInt(); // Extract minutes part and convert to integer
  return hours * 60 + minutes; // Convert hours to minutes and add the minutes
}

/**
 * @brief Adds a new pill to the list if there's space.
 * 
 * @param name The name of the pill.
 * @param time The time for the pill in "HH:MM" format.
 * @param color An integer representing the color associated with the pill.
 * @param music An integer representing the music associated with the pill.
 */
void addPill(String name, String time, int color, int music) { 
  if (numPills < MAX_PILLS) // Check if there's space to add a new pill
  {
    pills[numPills].name = name; // Assign the name to the new pill
    pills[numPills].hourMinutes = convertTimeToMinutes(time); // Convert time to minutes and assign
    pills[numPills].color = color; // Assign the color to the new pill
    pills[numPills].music = music; // Assign the music to the new pill
    numPills++; // Increment the number of pills
  }
}

/**
 * @brief Processes input string to add a new pill if it's unique.
 * 
 * @param input A String in the format "name,time,color,music".
 */
void processInput(String input) 
{
  int firstComma = input.indexOf(','); // Find the first comma
  int secondComma = input.indexOf(',', firstComma + 1); // Find the second comma
  int thirdComma = input.indexOf(',', secondComma + 1); // Find the third comma

  String name = input.substring(0, firstComma); // Extract the name
  String time = input.substring(firstComma + 1, secondComma); // Extract the time
  int color = input.substring(secondComma + 1, thirdComma).toInt(); // Extract the color
  int music = input.substring(thirdComma + 1).toInt(); // Extract the music

  bool ok = true; // Flag to check if the pill is unique
  for(int i = 0; i < numPills; i++) // Iterate over existing pills
  {
    if(name.equals(pills[i].name)) // Check if the name matches
    {
      if(convertTimeToMinutes(time) == pills[i].hourMinutes) // Check if the time matches
      {
        if((pills[i].color == color) && (pills[i].music == music)) // Check if color and music match
        {
          ok = false; // The pill already exists
          break; // Exit loop
        }
      }
    }
  }
  if(ok) // If the pill is unique
  {
    addPill(name, time, color, music); // Add the new pill
  }
}

/**
 * @brief Gets the current time in minutes since midnight.
 * 
 * @return int The current time in minutes since midnight.
 */
int obtainCurrentTimeInMinutes() 
{
  DateTime now = rtc.now(); // Get the current date and time from the RTC
  char buf[20];
  sprintf(buf, "%02d/%02d/%02d %02d:%02d:%02d", now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());
  Serial.println(buf); // Print the current date and time to the serial monitor

  return now.hour() * 60 + now.minute(); // Convert the current time to minutes since midnight
}

/**
 * @brief Removes a pill from the list by its index.
 * 
 * @param input A String in the format "index", where index is the position of the pill to be removed.
 */
void removePill(String input)
{
  int firstComma = input.indexOf(','); // Find the comma (if any)
  int index = input.substring(0, firstComma).toInt(); // Extract the index to remove

  for (int j = index-1; j < numPills - 1; j++) // Shift pills down to remove the selected one
  {
    pills[j] = pills[j + 1]; // Shift the pill at j+1 to position j
  }
  numPills--; // Decrement the number of pills
  Serial.println(numPills); // Print the updated number of pills
}

/**
 * @brief Updates an existing pill based on the input string.
 * 
 * @param input A String in the format "index,name,time,color,music".
 */
void updatePill(String input)
{
  int firstComma = input.indexOf(','); // Find the first comma
  int secondComma = input.indexOf(',', firstComma + 1); // Find the second comma
  int thirdComma = input.indexOf(',', secondComma + 1); // Find the third comma
  int fourthComma = input.indexOf(',', thirdComma + 1); // Find the fourth comma

  int index = input.substring(0, firstComma).toInt(); // Extract the index of the pill to update
  String name = input.substring(firstComma + 1, secondComma); // Extract the new name
  String time = input.substring(secondComma + 1, thirdComma); // Extract the new time
  int color = input.substring(thirdComma + 1, fourthComma).toInt(); // Extract the new color
  int music = input.substring(fourthComma + 1).toInt(); // Extract the new music

  pills[index-1].name = name; // Update the name of the selected pill
  pills[index-1].hourMinutes = convertTimeToMinutes(time); // Update the time of the selected pill
  pills[index-1].color = color; // Update the color of the selected pill
  pills[index-1].music = music; // Update the music of the selected pill
}


int melody1[] = 
{ 
	 NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
  NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
  NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4,
  NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4,
  NOTE_G4, NOTE_C4,
  
  NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
  NOTE_D4,
  NOTE_F4, NOTE_AS3,
  NOTE_DS4, NOTE_D4, NOTE_F4, NOTE_AS3,
  NOTE_DS4, NOTE_D4, NOTE_C4,
  
  NOTE_G4, NOTE_C4,
  
  NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
  NOTE_D4,
  NOTE_F4, NOTE_AS3,
  NOTE_DS4, NOTE_D4, NOTE_F4, NOTE_AS3,
  NOTE_DS4, NOTE_D4, NOTE_C4,
  NOTE_G4, NOTE_C4,
  NOTE_DS4, NOTE_F4, NOTE_G4,  NOTE_C4, NOTE_DS4, NOTE_F4,
  
  NOTE_D4,
  NOTE_F4, NOTE_AS3,
  NOTE_D4, NOTE_DS4, NOTE_D4, NOTE_AS3,
  NOTE_C4,
  NOTE_C5,
  NOTE_AS4,
  NOTE_C4,
  NOTE_G4,
  NOTE_DS4,
  NOTE_DS4, NOTE_F4,
  NOTE_G4,
  
  NOTE_C5,
  NOTE_AS4,
  NOTE_C4,
  NOTE_G4,
  NOTE_DS4,
  NOTE_DS4, NOTE_D4,
  NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4, NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4,
  NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4, NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4,
  
  REST, NOTE_GS5, NOTE_AS5, NOTE_C6, NOTE_G5, NOTE_GS5, NOTE_AS5,
  NOTE_C6, NOTE_G5, NOTE_GS5, NOTE_AS5, NOTE_C6, NOTE_G5, NOTE_GS5, NOTE_AS5
}; 
int duration1[] = 
{ 
	 8, 8, 16, 16, 8, 8, 16, 16,
  8, 8, 16, 16, 8, 8, 16, 16,
  8, 8, 16, 16, 8, 8, 16, 16,
  8, 8, 16, 16, 8, 8, 16, 16,
  4, 4,
  
  16, 16, 4, 4, 16, 16,
  1,
  4, 4,
  16, 16, 4, 4,
  16, 16, 1,
  
  4, 4,
  
  16, 16, 4, 4, 16, 16,
  1,
  4, 4,
  16, 16, 4, 4,
  16, 16, 1,
  4, 4,
  16, 16, 4, 4, 16, 16,
  
  2,
  4, 4,
  8, 8, 8, 8,
  1,
  2,
  2,
  2,
  2,
  2,
  4, 4,
  1,
  
  2,
  2,
  2,
  2,
  2,
  4, 4,
  8, 8, 16, 16, 8, 8, 16, 16,
  8, 8, 16, 16, 8, 8, 16, 16,
  
  4, 16, 16, 8, 8, 16, 16,
  8, 16, 16, 16, 8, 8, 16, 16
}; 
int melody2[]=
{
REST, NOTE_D4,
  NOTE_G4, NOTE_AS4, NOTE_A4,
  NOTE_G4, NOTE_D5,
  NOTE_C5, 
  NOTE_A4,
  NOTE_G4, NOTE_AS4, NOTE_A4,
  NOTE_F4, NOTE_GS4,
  NOTE_D4, 
  NOTE_D4,
  
  NOTE_G4, NOTE_AS4, NOTE_A4,
  NOTE_G4, NOTE_D5,
  NOTE_F5, NOTE_E5,
  NOTE_DS5, NOTE_B4,
  NOTE_DS5, NOTE_D5, NOTE_CS5,
  NOTE_CS4, NOTE_B4,
  NOTE_G4,
  NOTE_AS4,
   
  NOTE_D5, NOTE_AS4,
  NOTE_D5, NOTE_AS4,
  NOTE_DS5, NOTE_D5,
  NOTE_CS5, NOTE_A4,
  NOTE_AS4, NOTE_D5, NOTE_CS5,
  NOTE_CS4, NOTE_D4,
  NOTE_D5, 
  REST, NOTE_AS4,  
  
  NOTE_D5, NOTE_AS4,
  NOTE_D5, NOTE_AS4,
  NOTE_F5, NOTE_E5,
  NOTE_DS5, NOTE_B4,
  NOTE_DS5, NOTE_D5, NOTE_CS5,
  NOTE_CS4, NOTE_AS4,
  NOTE_G4
};
int duration2[]=
{
2, 4,
  4, 8, 4,
  2, 4,
  2, 
  2,
  4, 8, 4,
  2, 4,
  1, 
  4,
  
  4, 8, 4,
  2, 4,
  2, 4,
  2, 4,
  4, 8, 4,
  2, 4,
  1,
  4,
   
  2, 4,
  2, 4,
  2, 4,
  2, 4,
  4, 8, 4,
  2, 4,
  1, 
  4, 4,  
  
  2, 4,
  2, 4,
  2, 4,
  2, 4,
  4, 8, 4,
  2, 4,
  1
};
int melody3[]=
{
NOTE_AS4, REST, NOTE_AS4, REST, NOTE_AS4, REST, NOTE_AS4, REST,
  NOTE_AS4, NOTE_B4, NOTE_DS5,
  NOTE_AS4, REST, NOTE_AS4, REST,
  NOTE_AS4, NOTE_B4, NOTE_DS5,
  NOTE_AS4, REST, NOTE_AS4, REST,
  NOTE_AS4, NOTE_B4, NOTE_DS5,
  NOTE_AS4, REST, NOTE_AS4, REST,
  NOTE_AS4, NOTE_B4, NOTE_DS5,
  NOTE_F5, REST, NOTE_F5, REST,
  NOTE_GS5, NOTE_FS5, NOTE_F5,
  NOTE_AS4, REST, NOTE_AS4, REST,
  NOTE_GS5, NOTE_FS5, NOTE_F5,
  NOTE_AS4, REST, NOTE_AS4, REST,
  NOTE_AS4, NOTE_B4, NOTE_DS5,
  NOTE_AS4, REST, NOTE_AS4, REST,
  NOTE_AS4, NOTE_B4, NOTE_DS5,
  NOTE_AS4, REST, NOTE_AS4, REST,
  REST
};
int duration3[]=
{
  4, 4, 4, 4, 4, 4, 4, 4,
  3, 3, 4,
  4, 4, 4, 4,
  3, 3, 4,
  4, 4, 4, 4,
  3, 3, 4,
  4, 4, 4, 4,
  3, 3, 4,
  4, 4, 4, 4,
  3, 3, 4,
  4, 4, 4, 4,
  3, 3, 4,
  4, 4, 4, 4,
  3, 3, 4,
  4, 4, 4, 4,
  3, 3, 4,
  4, 4, 4, 4,
  1
};

/**
 * @brief Reads the current humidity and temperature from a DHT sensor and sends the values via a messaging protocol.
 * 
 * This function reads humidity and temperature values from a DHT sensor. It checks if the readings are valid,
 * and if they are, it constructs a string with these values and sends it using the `sendMessageByTopic` function.
 */
void getCurrentTemperatureAndHumidity()
{
  float humidity = dht.readHumidity(); // Read the humidity from the DHT sensor
  float temperature = dht.readTemperature(); // Read the temperature from the DHT sensor (Celsius by default)
  
  if (isnan(humidity) || isnan(temperature)) // Check if either reading failed
  {
    Serial.println("Failed to read from DHT sensor!"); // Print an error message if the readings are invalid
    return; // Exit the function if readings are invalid
  }
  
  String humAndTempMeassure = String(humidity) + "," + String(temperature); // Format humidity and temperature into a comma-separated string

  sendMessageByTopic(TOPIC_TEMP_HUM, humAndTempMeassure); // Send the formatted string via the messaging protocol
}

/**
 * @brief Plays a melody using a buzzer.
 * 
 * This function plays a sequence of tones (a melody) on a buzzer. The melody and its durations are passed as arrays,
 * and the size of the melody is used to control the loop. The function continues playing notes until the melody is finished
 * or a button is pressed.
 * 
 * @param melody An array of melody notes (frequencies).
 * @param duration An array of durations corresponding to each note.
 * @param size The number of notes in the melody.
 */
void playMelody(int * melody, int * duration, int size) 
{
  for (int note = 0; (note < size) && (button_pressed == false); note++) // Loop through each note in the melody
  {
    int noteDuration = 1000 / duration[note]; // Calculate the duration of the note
    tone(BUZZER_PIN, melody[note], noteDuration); // Play the note on the buzzer
    int pauseBetweenNotes = noteDuration * 1.30; // Calculate pause duration between notes
    delay(pauseBetweenNotes); // Wait for the pause duration
    noTone(BUZZER_PIN); // Stop playing the tone
  }
}

/**
 * @brief Plays a specific melody based on the provided identifier.
 * 
 * This function selects and plays one of several predefined melodies based on the input identifier. The melodies are played
 * using the `playMelody` function. Each case corresponds to a different melody.
 * 
 * @param melody An integer identifying which melody to play (1, 2, or 3).
 */
void playBuzzer(int melody)
{
  switch (melody) 
  {
      case 1:
        Serial.println("Music 1"); // Print which melody is being played
        playMelody(melody1, duration1, sizeof(duration1) / sizeof(int)); // Play the first melody
        break;

      case 2:
        Serial.println("Music 2"); // Print which melody is being played
        playMelody(melody2, duration2, sizeof(duration2) / sizeof(int)); // Play the second melody
        break;

      case 3:
        Serial.println("Music 3"); // Print which melody is being played
        playMelody(melody3, duration3, sizeof(duration3) / sizeof(int)); // Play the third melody
        break;

      default:
        // Optionally, handle invalid input here
        //Serial.println("NO Music"); // Uncomment to indicate no music for invalid input
        break;
    }
}

/**
 * @brief Opens a servo motor to a specified position.
 * 
 * This function gradually opens a servo motor to a position of 90 degrees. The servo is attached, and the function uses
 * a loop to increment the position from 0 to 90 degrees in 1-degree steps, with a short delay between each step to allow
 * the servo to reach each position.
 */
void servoOpen()
{
   servo_opened = true; // Set the flag indicating the servo is open
   Serial.println("Servo opening"); // Print a message indicating the servo is opening
   myServo.setPeriodHertz(50);           // Set the servo signal frequency to 50 Hz (standard for servos)
   myServo.attach(SERVO_PIN, 500, 2500);  // Attach the servo to the specified pin with min and max pulse widths

  for (int posVal = 0; posVal <= 90; posVal += 1) 
  { 
    // Gradually move the servo from 0 to 90 degrees
    myServo.write(posVal);       // Set the servo position
    delay(15);                   // Wait 15 ms for the servo to reach the position
  } 
}

/**
 * @brief Closes a servo motor from a specified position.
 * 
 * This function gradually closes a servo motor from a position of 90 degrees back to 0 degrees. The servo is attached,
 * and the function uses a loop to decrement the position from 90 to 0 degrees in 1-degree steps, with a short delay between
 * each step to allow the servo to reach each position.
 */
void servoClose()
{
   Serial.println("Servo closing"); // Print a message indicating the servo is closing
   myServo.setPeriodHertz(50);           // Set the servo signal frequency to 50 Hz (standard for servos)
   myServo.attach(SERVO_PIN, 500, 2500);  // Attach the servo to the specified pin with min and max pulse widths
  for (int posVal = 90; posVal >= 0; posVal -= 1) 
  { 
    // Gradually move the servo from 90 to 0 degrees
    myServo.write(posVal);       // Set the servo position
    delay(15);                   // Wait 15 ms for the servo to reach the position
  }
}

/**
 * @brief Sets the RGB LED color based on the provided identifier.
 * 
 * This function controls the color of an RGB LED by adjusting the brightness of the red, green, and blue components. 
 * It turns on the LED with a specific color and handles turning off the light if it was previously off.
 * 
 * @param color An integer identifying the color to display (1 for red, 2 for green, 3 for blue).
 */
void RGB(int color)
{
   switch (color) {
      case 1:
        analogWrite(PIN_RED, 200); // Set red LED to a brightness of 200
        break;

      case 2:
        analogWrite(PIN_GREEN, 200); // Set green LED to a brightness of 200
        break;

      case 3:
        analogWrite(PIN_BLUE, 200); // Set blue LED to a brightness of 200
        break;

      default:
        Serial.println("Invalid input"); // Print an error message for invalid color input
        break;
    }

    if(light_on == false) // Check if the light is currently off
    {
      analogWrite(PIN_BLUE, 0); // Turn off the blue LED
      analogWrite(PIN_GREEN, 0); // Turn off the green LED
      analogWrite(PIN_RED, 0); // Turn off the red LED

      light_on = true; // Set the flag indicating the light is on
      set_value(-1, &current_pill); // Optionally call `set_value` with -1 and `current_pill`
    }
}

/*** End of file ****/