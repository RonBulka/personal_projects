#ifndef __STUDENT__
#define __STUDENT__

#include "stack.h"
#include <stdlib.h>
#include <string.h>

typedef struct student {
	char *name;
	int age;
    int id;
}*pstudent_t, student_t;

/**
 * @brief creates a clone of a student
 * 
 * @param pstudent - pointer to student to clone
 * @return elem_t if success, NULL if fail
 */
elem_t student_clone (elem_t pstudent);
/* ------------------------------------------ */
/* Add here the reset of the declarations     */
/* ------------------------------------------ */

/**
 * @brief destroys a student
 * 
 * @param pstudent - pointer to student to destroy
 * @return None 
 */
void student_destroy (elem_t pstudent);

/**
 * @brief print the data of a student in the format:
 * student name: <name>, age: <age>, id: <id>.
 * 
 * @param pstudent - pointer to student to print data of
 * @return None
 */
void student_print(elem_t pstudent);

#endif /* __STUDENT__ */ 