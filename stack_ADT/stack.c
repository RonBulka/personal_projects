#include "stack.h"

typedef struct node {
    elem_t data;
    struct node* next;
} node, *p_node;


typedef struct stack {
    p_node head;
    size_t curr_size;
    size_t max_size;
    clone_t clone;
    destroy_t destroy;
    print_t print;
} stack, *pstack_t;


/* Node functions */

p_node node_create(elem_t data, clone_t pfunc_clone);
Result node_destroy(p_node node, destroy_t pfunc_destroy);
void node_print(p_node node, print_t pfunc_print);

/**
 * @brief creates a new node
 * 
 * @param data - the data to store in the node
 * @param pfunc_clone - a function pointer to a function that clones an element
 * @return p_node - a pointer to the new node, or NULL if failed
 */
p_node node_create(elem_t data, clone_t pfunc_clone){
    if (data == NULL){
        return NULL;
    }

    p_node pnode = (p_node)malloc(sizeof(node));
    if (pnode == NULL){
        fprintf(stderr, "Error: malloc failed on node_create\n");
        return NULL;
    }

    pnode->data = pfunc_clone(data);
    if (pnode->data == NULL){
        fprintf(stderr, "Error: pfunc_clone failed\n");
        free(pnode);
        return NULL;
    } 
    return pnode;
}

/**
 * @brief destroys a node
 * 
 * @param node - pointer to the node to destroy
 * @param pfunc_destroy - a function pointer to a function that destroys
 * the data.
 * @return Result - SUCCESS if the node was destroyed successfully,
 * FAIL otherwise.
 */
Result node_destroy(p_node node, destroy_t pfunc_destroy){
    if (node == NULL){
        return SUCCESS;
    }

    pfunc_destroy(node->data);
    free(node);

    return SUCCESS;
}

/**
 * @brief prints the data of a node
 * 
 * @param node - pointer to the node to print data of
 * @param pfunc_print - a function pointer to a function that prints data
 */
void node_print(p_node node, print_t pfunc_print){
    if (node == NULL){
        return;
    }
    pfunc_print(node->data);
    
    node_print(node->next, pfunc_print);
}


/* Stack functions */

pstack_t stack_create(size_t max_num_of_elem,
                      clone_t pfunc_clone,
                      destroy_t pfunc_destroy,
                      print_t pfunc_print)
{
    if (pfunc_clone == NULL){
        fprintf(stderr, "Error: pfunc_clone is NULL\n");
        return NULL;
    }
    else if (pfunc_destroy == NULL){
        fprintf(stderr, "Error: pfunc_destroy is NULL\n");
        return NULL;
    }
    else if (pfunc_print == NULL){
        fprintf(stderr, "Error: pfunc_print is NULL\n");
        return NULL;
    }
    else if (max_num_of_elem < 1){
        fprintf(stderr, "Error: max_num_of_elem is less than 1\n");
        return NULL;
    }

    pstack_t pstack = (pstack_t)malloc(sizeof(stack));
    if (pstack == NULL){
        fprintf(stderr, "Error: malloc failed on stack_create\n");
        return NULL;
    }

    pstack->clone = pfunc_clone;
    pstack->destroy = pfunc_destroy;
    pstack->print = pfunc_print;
    pstack->max_size = max_num_of_elem;
    pstack->curr_size = 0;
    pstack->head = NULL;

    return pstack;
}


Result stack_destroy(pstack_t stack)
{
    if (stack == NULL){
        fprintf(stderr, "Error:(stack_destroy) stack is NULL\n");
        return FAIL;
    }
    while(stack->head != NULL){
        stack_pop(stack);
    }
    free(stack);
    return SUCCESS;
}


Result stack_push(pstack_t stack, elem_t elem)
{

    if (stack == NULL){
        fprintf(stderr, "Error:(stack_push) stack is NULL\n");
        return FAIL;
    }
    else if (!stack_capacity(stack)){
        fprintf(stderr, "Error:(stack_push) stack is full\n");
        return FAIL;
    }

    p_node pnode = node_create(elem, stack->clone);
    if (pnode == NULL){
        // error message printed in node_create
        return FAIL;
    }

    pnode->next = stack->head;
    stack->head = pnode;
    (stack->curr_size)++;

    return SUCCESS;
}


void stack_pop(pstack_t stack)
{
    if (stack == NULL){
        fprintf(stderr, "Error:(stack_pop) stack is NULL\n");
        return;
    }
    else if (stack->head == NULL){
        fprintf(stderr, "Error:(stack_pop) stack is empty\n");
        return;
    }

    p_node new_head = (stack->head)->next;
    node_destroy(stack->head, stack->destroy);
    (stack->curr_size)--;

    stack->head = new_head;
    
}


elem_t stack_peek(pstack_t stack)
{
    if (stack == NULL){
        fprintf(stderr, "Error:(stack_peek) stack is NULL\n");
        return NULL;
    }
    else if (stack->head == NULL){
        fprintf(stderr, "Error:(stack_peek) stack is empty\n");
        return NULL;
    }

    return stack->head->data;
}


size_t stack_size(pstack_t stack)
{
    if (stack == NULL){
        fprintf(stderr, "Error:(stack_size) stack is NULL\n");
        return 0;
    }

    return stack->curr_size;
}


bool stack_is_empty(pstack_t stack)
{
    if (stack == NULL){
        fprintf(stderr, "Error:(stack_is_empty) stack is NULL\n");
        return false;
    }
    
    if(stack->head == NULL){
        return true;
    }

    return false;
}


size_t stack_capacity(pstack_t stack)
{
    if (stack == NULL){
        fprintf(stderr, "Error:(stack_capacity) stack is NULL\n");
        return 0;
    }
    return stack->max_size - stack->curr_size;
}


void stack_print(pstack_t stack)
{
    if (stack == NULL){
        fprintf(stderr, "Error:(stack_print) stack is NULL\n");
        return;
    }
    node_print(stack->head, stack->print);
}