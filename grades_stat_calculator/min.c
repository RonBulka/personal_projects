#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {MAX_GRADE = 100, MIN_GRADE = 0, EXPECTED_ARGS = 2};

int c_min(FILE* fp);

/**
 * @brief Handles the command line arguments and calls c_min.
 * @note This function reads the grades from the given file stream, 
 * identifies the minimum grade, and prints it to stdout.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return Exit code.
 */
int main(int argc, char** argv){
    FILE *fp;
    // handle args
    if (argc == 1 || !strcmp(argv[1], "-")){
        fp = stdin;
    } else if (argc > EXPECTED_ARGS){
        fprintf(stderr, "Error: too many arguments");
        return 1;
    } else {
        fp = fopen(argv[1], "r");
    }

    if (fp == NULL){
        fprintf(stderr, "Error: cannot open file %s\n" , argv[1]);
        return 1;
    }

    int err = c_min(fp);
    fclose(fp);

    return err;
}



/**
 * @brief Calculates the minimum grade from the given file stream.
 * @note This function reads the grades from the given file stream,
 * identifies the minimum grade, and prints it to stdout. It also handles
 * invalid input and prints error messages to stderr.
 *
 * @param fp The file pointer to read the grades from.
 */
int c_min(FILE* fp){
    int err = 0;
    int grade = 0;
    int min = MAX_GRADE;
    int line_number = 0;

    while (1){
        err = fscanf(fp, "%d", &grade);
        // handle EOF
        if (err == EOF){
            break;
        }
        // handle invalid input
        else if(err != 1){
            fprintf(stderr, "Error: fscanf failed. line number: %d\n"
                    , ++line_number);
            return 1;
        }
        else if (grade < MIN_GRADE || grade > MAX_GRADE){
            fprintf(stderr, "Error: invalid input. line number: %d\n", 
                    ++line_number);
            continue;
        }

        if (grade < min){
            min = grade;
        }

        line_number++;
    }

    fprintf(stdout, "%d", min);
    return 0;
}