/**
 * @file  prot_var_int.ino
 * @brief Implementation of functions for a protected integer variable
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2024 Andrea Nieto Gil. All rights reserved.
 */

/**
 * @brief Gets the value of the protected variable
 * @param data Pointer to the element where the content of the variable will be stored
 * @param var_prot Pointer to the protected variable 
 * @return 0 if the value is obtained correctly
 */
int8_t get_value(int * data, Var_Prot_Int_t * var_prot)
{
    portENTER_CRITICAL(&(var_prot->taskMux)); // Enter critical section to protect access
    *data = var_prot->data; // Retrieve the protected variable's value
    portEXIT_CRITICAL(&(var_prot->taskMux)); // Exit critical section

    return 0;
}

/**
 * @brief Sets the value of the protected variable
 * @param data Value to set the variable to
 * @param var_prot Pointer to the protected variable 
 * @return 0 if the value is set correctly
 */
int8_t set_value(int data, Var_Prot_Int_t * var_prot)
{
    portENTER_CRITICAL(&(var_prot->taskMux)); // Enter critical section to protect access
    var_prot->data = data; // Set the protected variable's value
    portEXIT_CRITICAL(&(var_prot->taskMux)); // Exit critical section

    return 0;
}

/*** End of file ****/