/**
 * @file  prot_var_int.h
 * @brief Definitions and functions for a protected integer variable
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2024 Andrea Nieto Gil. All rights reserved.
 */

#ifndef VAR_PROT_INT_H
#define VAR_PROT_INT_H

/**
 * @brief   Structure to instantiate a protected integer variable
 * @details Stores the content of the variable and the mutex to protect access
 */
typedef struct Var_Prot_Int
{
    int data = 0; ///< Protected integer variable
    portMUX_TYPE taskMux = portMUX_INITIALIZER_UNLOCKED; ///< Mutex to protect access to the variable
} Var_Prot_Int_t;

// Functions for handling the protected variable

/**
 * @brief Gets the value of the protected variable
 * @param data Pointer to the element where the content of the variable will be stored
 * @param var_prot Pointer to the protected variable 
 * @return 0 if the value is obtained correctly
 */
int8_t get_value(int * data, Var_Prot_Int_t * var_prot);

/**
 * @brief Sets the value of the protected variable
 * @param data Value to set the variable to
 * @param var_prot Pointer to the protected variable 
 * @return 0 if the value is set correctly
 */
int8_t set_value(int data, Var_Prot_Int_t * var_prot);

#endif // VAR_PROT_INT_H

/*** End of file ****/