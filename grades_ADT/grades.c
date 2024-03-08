#include "grades.h"
#include "linked-list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum { SUCCESS = 0, FAIL};

typedef struct course{
    char* name;
    int grade;
}course, *p_course;


typedef struct student{
    int id;
    char* name;
    struct list* courses;
} student, *p_student;


typedef struct grades{
    struct list* students_list;
} grades, *p_grades;


// --------- list ---------

/**
 * @brief clones a list
 * 
 * @param[in] input_list - the list to clone
 * @param[out] output_list - the list to clone to. allocated elsewhere
 * @return int - 0 on success, 1 on failure
 */
int list_clone(struct list* input_list, struct list* output_list){
    if (input_list == NULL){
        return FAIL;
    }
    if (output_list == NULL){
        return FAIL;
    }

    // iterate over all elements in input list and clone them
    for (struct iterator* it = list_begin(input_list);
        it != NULL;
        it = list_next(it))
    {       
        void* element = list_get(it);
        if (element != NULL && list_push_back(output_list, element)){
            return FAIL;
        }
    }

    return SUCCESS;
}


// --------- course ---------

/**
 * @brief initializes a course
 * 
 * @param name - the name of the course
 * @param grade - the grade of the course
 * @return an instance of a course, or NULL if failed
 */
p_course course_init(const char* name, int grade){
    p_course new_course = (p_course) malloc(sizeof(struct course));
    if(new_course == NULL){
        return NULL;
    }

    new_course->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
    if(new_course->name == NULL){
        free(new_course);
        return NULL;
    }
    
    strcpy(new_course->name, name);
    new_course->grade = grade;

    return new_course;
}


/**
 * @brief clones a course
 * 
 * @param[in] input - the course to clone
 * @param[out] output - the course to clone to. allocated by the function
 * @return int - 0 on success, 1 on failure
 */
int course_clone(void* input, void* *output){
    if (input == NULL){
        return FAIL;
    }
    p_course input_course = (p_course)input;
    if (input_course->name == NULL){
        return FAIL;
    }
    else if (input_course->grade < 0 || input_course->grade > 100){
        return FAIL;
    }
    p_course output_course = (p_course)malloc(sizeof(struct course));
    if(output_course == NULL){
        return FAIL;
    }
    output_course->name = (char*)malloc(sizeof(char) * 
                                        (strlen(input_course->name) + 1));
    if(output_course->name == NULL){
        free(output_course);
        return FAIL;
    }
    strcpy(output_course->name, input_course->name);
    output_course->grade = input_course->grade;
    *output = output_course;
    return SUCCESS;
}


/**
 * @brief destroys a course (destructor)
 * 
 * @param course - the course to destroy
 */
void course_destroy(void* course){
    if (course == NULL){
        return;
    }
    p_course des_course = (p_course)course;

    free(des_course->name);
    free(des_course);
}


// --------- student ---------

/**
 * @brief initializes a student
 * 
 * @param id - the id of the student
 * @param name - the name of the student
 * @return an instance of a student, or NULL if failed
 */
p_student student_init(int id, const char* name){
    p_student student = (p_student) malloc(sizeof(struct student));
    if(student == NULL){
        return NULL;
    }

    student->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
    if(student->name == NULL){
        return NULL;
    }
    
    // need to check if stuff failed
    strcpy(student->name, name);
    student->id = id;
    student->courses = list_init(&course_clone, &course_destroy);
    if (student->courses == NULL){
        return NULL;
    }

    return student;
}


/**
 * @brief clones a student
 * 
 * @param[in] element - the student to clone
 * @param[out] output - the student to clone to. allocated by the function
 * @return int - 0 on success, 1 on failure
 */
int student_clone(void* element, void* *output){
    if (element == NULL){
        return FAIL;
    }
    p_student student = (p_student) element;
    if (student->name == NULL){
        return FAIL;
    }
    else if (student->id < 0){
        return FAIL;
    }
    else if (student->courses == NULL){
        return FAIL;
    }
    
    p_student new_student = student_init(student->id, student->name);

    if(new_student == NULL){
        return FAIL;
    }

    list_clone(student->courses, new_student->courses);
    *output = new_student;
    return SUCCESS;
}


/**
 * @brief destroys a student (destructor)
 * 
 * @param element - the student to destroy
 */
void student_destroy(void *element){
    if (element == NULL){
        return;
    }

    p_student student = (p_student) element;
    
    free(student->name);
    list_destroy(student->courses);
    
    free(student);
    
}


/**
 * @brief adds a grade to a student
 * 
 * @param student - the student to add the grade to
 * @param name - the name of the course
 * @param grade - the grade of the course
 * @return int - 0 on success, 1 on failure
 */
int student_add_grade(p_student student, const char* name, int grade){
    if(student == NULL || name == NULL || (grade < 0 || grade > 100)){
        return FAIL;
    }
    
    // check if course already exists in student
    for (struct iterator* it = list_begin(student->courses);
            it != NULL;
            it = list_next(it))
    {
        p_course curr_course = (p_course)list_get(it);
        if (curr_course != NULL && !strcmp(name, curr_course->name)){
            return FAIL;
        }
    }
    
    // add course to student
    p_course course = course_init(name, grade);
    if (course == NULL){
        return FAIL;
    }
    int success = list_push_back(student->courses, course);
    course_destroy(course);
    return success;
}


/**
 * @brief calculates the average grade of a student
 * 
 * @param student - the student to calculate the average grade of
 * @return float - the average grade of the student, or -1 on failure
 */
float student_avg(p_student student){
    int size = list_size(student->courses);
    if (size == 0){
        return 0;
    }
    float sum = 0;

    // go over all courses and sum the grades
    for (struct iterator* it = list_begin(student->courses);
         it != NULL;
         it = list_next(it))
    {
        p_course course = (p_course)list_get(it);
        if (course != NULL){
            sum += course->grade;
        }
    }

    return sum / size;
}


/**
 * @brief prints a student
 * 
 * @param student - the student to print
 */
void student_print(p_student student){
    if (student == NULL){
        return;
    }
    // print student name and id
    printf("%s %d:", student->name, student->id);
    struct iterator* it = list_begin(student->courses);
    if (it == NULL){
        printf("\n");
        return;
    }
    // print all the courses and grades student holds
    else{
        p_course curr_course = (p_course)list_get(it);
        if (curr_course != NULL){
            printf(" %s %d", curr_course->name, curr_course->grade);
            it = list_next(it);
        }
    }
    while(it != NULL){
        p_course curr_course = (p_course)list_get(it);
        if (curr_course != NULL){
            printf(", %s %d", curr_course->name, curr_course->grade);
            it = list_next(it);
        }
    }
    printf("\n");
    return;
}


// --------- grades ---------

/**
 * @brief initializes a grades struct
 * 
 * @return an instance of a grades struct, or NULL if failed
 */
struct grades* grades_init(){
    p_grades grades = (p_grades)malloc(sizeof(struct grades));
    if(grades == NULL){
        return NULL;
    }

    struct list* grades_list = list_init(&student_clone, &student_destroy);
    if(grades_list == NULL){
        free(grades);
        return NULL;
    }
    grades->students_list = grades_list;

    return grades;
}


void grades_destroy(grades *grades){
    if (grades == NULL){
        return;
    }

    list_destroy(grades->students_list);
    free(grades);

}


int grades_add_student(grades *grades, const char *name, int id){
    if(grades == NULL){
        return FAIL;
    }

    // iterate over all students and check if student with same id exists
    for (struct iterator* it = list_begin(grades->students_list);
            it != NULL;
            it = list_next(it))
    {
        p_student curr_student = (p_student)list_get(it);
        if (curr_student != NULL && curr_student->id == id){
            return FAIL;
        }
    }

    p_student student = student_init(id, name);
    if (student == NULL){
        return FAIL;
    }
    int success = list_push_back(grades->students_list, student);
    student_destroy(student);
    return success;
}


int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade)
{
    if (grade < 0 || grade > 100 ||
        grades == NULL){
        return FAIL;
    }

    // iterate over all students and find the one with the correct id
    for (struct iterator* it = list_begin(grades->students_list);
            it != NULL;
            it = list_next(it))
    {
        p_student curr_student = (p_student)list_get(it);
        if (curr_student != NULL && curr_student->id == id){
            return student_add_grade(curr_student, name, grade);
        }
    }

    return FAIL;
    
}

float grades_calc_avg(struct grades *grades, int id, char **out){
    if(grades == NULL){
        *out = NULL;
        return -1;
    }

    p_student student = NULL;

    // iterate over all students and find the one with the correct id
    for (struct iterator* it = list_begin(grades->students_list);
            it != NULL;
            it = list_next(it))
    {
        p_student curr_student = (p_student)list_get(it);
        if (curr_student != NULL && curr_student->id == id){
            student = curr_student;
        }
    }
    // student not found
    if (student == NULL){
        *out = NULL;
        return -1;
    }
    
    float avg = student_avg(student);
    if (avg < 0){
        *out = NULL;
        return -1;
    }
    
    *out = (char*)malloc(sizeof(char) * (strlen(student->name) + 1));
    if(*out == NULL){
        return -1;
    }

    strcpy(*out, student->name);
    return avg;
}


int grades_print_student(struct grades *grades, int id){
    if (grades == NULL){
        return FAIL;
    }
    // iterate over all students and find the one with the correct id
    for (struct iterator* it = list_begin(grades->students_list);
         it != NULL;
         it = list_next(it))
    {
        p_student curr_student = (p_student)list_get(it);
        if (curr_student != NULL && curr_student->id == id){
            student_print(curr_student);
            return SUCCESS;
        }
    }
    // student not found
    return FAIL;
}

int grades_print_all(struct grades *grades){
    if (grades == NULL){
        return FAIL;
    }
    // iterate over all students and print them
    for (struct iterator* it = list_begin(grades->students_list);
         it != NULL;
         it = list_next(it))
    {
        p_student curr_student = (p_student)list_get(it);
        if (curr_student != NULL){
            student_print(curr_student);
        }
    }

    return SUCCESS;
}