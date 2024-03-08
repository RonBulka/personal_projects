#ifndef __STACK__
#define __STACK__

#include <stddef.h>  /* size_t */
#include <stdbool.h> /* bool   */ 
#include <stdlib.h>
#include <stdio.h>

typedef enum { SUCCESS = 0, FAIL } Result; 

typedef struct stack *pstack_t;
typedef void *elem_t;
typedef elem_t (*clone_t)(elem_t e);
typedef void (*destroy_t)(elem_t e);
typedef void (*print_t)(elem_t e);


/* ------------------------------------------ */
/* Add here declarations of function pointers */
/* ------------------------------------------ */

/**
* @fn pstack_t stack_create(size_t max_num_of_elem,
*                           clone_t p_func_clone,
*                           destroy_t p_func_destroy,
*                           print_t p_func_print);
* @brief creates a new user_stack. user_stack must be destroyed by
* stack_destroy.
* @param max_num_of_elem - the maximum number of elements that can be stored in
* the user_stack1
* @param p_func_clone - a function pointer to a function that clones an element
* @param p_func_destroy - a function pointer to a function that destroys an
* element
* @param p_func_print - a function pointer to a function that prints an element
* @return a pointer to the new user_stack, or NULL if an allocation error
*/
pstack_t stack_create(size_t max_num_of_elem,
                      clone_t pfunc_clone,
                      destroy_t pfunc_destroy,
                      print_t pfunc_print);

/**
 * @fn void stack_destroy(pstack_t s);
 * @brief destroys the user_stack. free all the memory of the elements in the
 * stack. user_stack must be a stack created by StackCreate otherwise
 * undefined behavior)
 * @param stack - pointer to the stack to destroy
 * @return SUCCESS if the stack was destroyed successfully, FAIL otherwise
 */
Result stack_destroy(pstack_t stack);
/* ------------------------------------------ */
/* Add here the rest of the declarations      */
/* ------------------------------------------ */

/**
 * @fn Result stack_push(pstack_t stack);
 * @brief pushes an element into the stack. the element is cloned before
 * @param stack - pointer to the stack to push element into
 * @param elem - the element to push
 * @return SUCCESS if the element was pushed successfully, FAIL otherwise
 */
Result stack_push(pstack_t stack, elem_t elem);

/**
 * @fn Result stack_pop(pstack_t stack);
 * @brief pops an element from the stack. the element is destroyed after
 * if the stack is empty, nothing happens
 * @param stack - pointer to the stack to pop element from
 * @return none
 */
void stack_pop(pstack_t stack);

/**
 * @fn elem_t stack_peek(pstack_t stack);
 * @brief returns the element at the top of the stack
 * @param stack - pointer to the stack 
 * @return top element of the stack, or NULL if the stack is empty
 */
elem_t stack_peek(pstack_t stack);

/**
 * @fn size_t stack_size(pstack_t stack);
 * @brief calcualate the number of elements in the stack
 * @param stack - pointer to the stack
 * @return the number of elements in the stack
 */
size_t stack_size(pstack_t stack);

/**
 * @fn bool stack_is_empty(pstack_t stack);
 * @brief check if the stack is empty
 * @param stack - pointer to the stack
 * @return true if the stack is empty, false otherwise
 */
bool stack_is_empty(pstack_t stack);

/**
 * @fn size_t stack_capacity(pstack_t stack);
 * @brief calculate the number of elements that can be added to the stack
 * @param stack - pointer to the stack
 * @return the maximum number of elements that can be added to the stack
 */
size_t stack_capacity(pstack_t stack);

/**
 * @fn void stack_print(pstack_t stack);
 * @brief Prints the stack items in the stack from top to bottom
 * @param stack - pointer to the stack to destroy
 * @return none
 */
void stack_print(pstack_t stack);

#endif /* __STACK__ */