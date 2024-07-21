/**
 * @file  buffer_int.h
 * @brief Definitions and functions for a protected circular integer buffer
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2024 Andrea Nieto Gil. All rights reserved.
 */

#ifndef BUFFER_INT_H
#define BUFFER_INT_H

#define BUFSIZE 10  ///< Buffer size

/**
 * @brief   Structure to instantiate a protected circular integer buffer
 * @details Stores buffer data, input and output positions,
 *          element counter, and a mutex to protect access
 */
typedef struct Buffer_Int 
{
	uint32_t buffer[BUFSIZE]; ///< Array of integers that stores the buffer data
	uint32_t bufIN = 0; ///< Input position for the next element
	uint32_t bufOUT = 0; ///< Output position for the next element 
	uint32_t counter = 0; ///< Number of elements in the buffer
	portMUX_TYPE taskMux = portMUX_INITIALIZER_UNLOCKED; ///< Mutex to protect buffer access
} Buffer_Circ_Int_t;

// Functions for handling the protected circular buffer

/**
 * @brief  Checks if the buffer is empty
 * @param  buff Pointer to the circular buffer
 * @return true if the buffer is empty, false otherwise
 */
bool isEmpty(Buffer_Circ_Int_t * buff);

/**
 * @brief  Checks if the buffer is full
 * @param  buff Pointer to the circular buffer
 * @return true if the buffer is full, false otherwise
 */
bool isFull(Buffer_Circ_Int_t * buff);

/**
 * @brief  Retrieves an element from the buffer
 * @param  buff Pointer to the circular buffer
 * @param  data Variable where the retrieved element will be stored
 * @return 0 if the element is retrieved successfully, -1 if the buffer is empty
 */
int8_t get_item(uint32_t * data, Buffer_Circ_Int_t * buff);

/**
 * @brief  Inserts an element into the buffer
 * @param  buff Pointer to the circular buffer
 * @param  data Element to be inserted into the buffer
 * @return 0 if the element is inserted successfully, -1 if the buffer is full
 */
int8_t put_item(uint32_t data, Buffer_Circ_Int_t * buff);

/**
 * @brief Gets the number of elements in the buffer
 * @param buff Pointer to the circular buffer
 * @return Number of elements in the buffer
 */
uint32_t number(Buffer_Circ_Int_t * buff);

/**
 * @brief Lists the contents of the buffer
 * @param buff Pointer to the circular buffer
 * @return 0 if the contents are listed successfully, -1 if the buffer is empty
 */
int8_t listBuffer(Buffer_Circ_Int_t * buff);

#endif // BUFFER_INT_H

/*** End of file ****/