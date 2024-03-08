#include "student.h"


elem_t student_clone (elem_t pstudent){
    student_t* student = (student_t*)pstudent;
    if (student->name == NULL){
        fprintf(stderr, "Error:(student_clone) student name is NULL\n");
        return NULL;
    }
    else if (student->age <= 0){
        fprintf(stderr, "Error:(student_clone) student age is invalid\n");
        return NULL;
    }
    else if (student->id <= 0){
        fprintf(stderr, "Error:(student_clone) student id is invalid\n");
        return NULL;
    }
    elem_t new_elem = (elem_t)malloc(sizeof(student_t));
    if(new_elem == NULL){
        fprintf(stderr, 
                "Error:(student_clone) malloc failed on elem allocation\n");
        return NULL;
    }
    student_t* new_student = (student_t*)new_elem;

    
    new_student->name = (char*)malloc(sizeof(char) *
                                      (strlen(student->name) + 1));
    if(new_student->name == NULL){
        fprintf(stderr, 
                "Error:(student_clone) malloc failed on name allocation\n");
        free(new_student);
        return NULL;
    }
    strcpy(new_student->name, student->name);
    new_student->age = student->age;
    new_student->id = student->id;

    return new_elem;
}


void student_destroy (elem_t pstudent){
    if (pstudent == NULL){
        fprintf(stderr, "Error:(student_destroy) pstudent is NULL\n");
        return;
    }
    student_t* student = (student_t*)pstudent;
    free(student->name);
    free(student);
}


void student_print(elem_t pstudent){
    if (pstudent == NULL){
        fprintf(stderr, "Error:(student_print) pstudent is NULL\n");
        return;
    }
    student_t* student = (student_t*)pstudent;
    if (student->name == NULL){
        fprintf(stderr, "Error:(student_print) student name is NULL\n");
        return;
    }
    else if (student->age <= 0){
        fprintf(stderr, "Error:(student_print) student age is invalid\n");
        return;
    }
    else if (student->id <= 0){
        fprintf(stderr, "Error:(student_print) student id is invalid\n");
        return;
    }
    printf("student name: %s, age: %d, id: %d.\n", student->name
                                                 , student->age
                                                 , student->id);
}

