/**
 * @file  buffer_int.ino
 * @brief Implementation of functions for a protected circular buffer
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2024 Andrea Nieto Gil. All rights reserved.
 */

#define BUFSIZE 10  ///< Buffer size

/**
 * @brief  Checks if the buffer is empty
 * @param  buff Pointer to the circular buffer
 * @return true if the buffer is empty, false otherwise
 */
bool isEmpty(Buffer_Circ_Int_t * buff)
{
	if(buff->counter == 0)  // Check if the counter is 0
	{
		return true;  // Buffer is empty
	}
	else
	{
		return false;  // Buffer is not empty
	}	
}

/**
 * @brief  Checks if the buffer is full
 * @param  buff Pointer to the circular buffer
 * @return true if the buffer is full, false otherwise
 */
bool isFull(Buffer_Circ_Int_t * buff)
{
	if(buff->counter == BUFSIZE)  // Check if the counter equals the buffer size
	{
		return true;  // Buffer is full
	}
	else
	{
		return false;  // Buffer is not full
	}	
}

/**
 * @brief  Retrieves an element from the buffer
 * @param  buff Pointer to the circular buffer
 * @param  data Variable where the retrieved element will be stored
 * @return 0 if the element is retrieved successfully, -1 if the buffer is empty
 */
int8_t get_item(uint32_t * data, Buffer_Circ_Int_t * buff )
{
	if(isEmpty(buff))  // Check if the buffer is empty
	{
		// No data in the buffer
		return -1;
	}
	else
	{
		portENTER_CRITICAL (&(buff->taskMux)); // Enter the critical section
		*data = buff->buffer[buff->bufOUT]; // Retrieve the element from the buffer
		buff->counter--; // Decrease the counter
		// Modular arithmetic for the buffer index
		buff->bufOUT = (buff->bufOUT + 1) % BUFSIZE;
		portEXIT_CRITICAL (&(buff->taskMux)); // Exit the critical section

		return 0;	
	}
}

/**
 * @brief  Inserts an element into the buffer
 * @param  buff Pointer to the circular buffer
 * @param  data Element to be inserted into the buffer
 * @return 0 if the element is inserted successfully, -1 if the buffer is full
 */
int8_t put_item(uint32_t data, Buffer_Circ_Int_t * buff )
{
	if(isFull(buff))  // Check if the buffer is full
	{
		// The buffer is full
		return -1;
	}
	else
	{
		portENTER_CRITICAL (&(buff->taskMux)); // Enter the critical section
		buff->buffer[buff->bufIN] = data;  // Insert the element into the buffer
		buff->counter++; // Increase the counter
		// Modular arithmetic for the buffer index
		buff->bufIN = (buff->bufIN + 1) % BUFSIZE;
		portEXIT_CRITICAL (&(buff->taskMux)); // Exit the critical section

		return 0;	
	}	
}

/**
 * @brief  Gets the number of elements in the buffer
 * @param  buff Pointer to the circular buffer
 * @return Number of elements in the buffer
 */
uint32_t number(Buffer_Circ_Int_t * buff)
{
	return buff->counter;  // Return the counter
}

/**
 * @brief  Lists the contents of the buffer
 * @param  buff Pointer to the circular buffer
 * @return 0 if the contents are listed successfully, -1 if the buffer is empty
 */
int8_t listBuffer(Buffer_Circ_Int_t * buff)
{
	if(isEmpty(buff))  // Check if the buffer is empty
	{
		// No data in the buffer
		return -1;
	}
	else
	{
		Serial.printf("Your buffer contains: ");
		for(uint32_t i = 0; i < BUFSIZE; i++)  // Loop through the buffer
		{
			Serial.printf("\t%d", buff->buffer[i]);  // Print each element
		}
		Serial.printf("\n");

		return 0;
	}
}

/*** End of file ****/