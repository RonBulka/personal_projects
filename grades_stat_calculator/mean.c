#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {MAX_GRADE = 100, MIN_GRADE = 0, EXPECTED_ARGS = 2};

int c_mean(FILE* fp);

/**
 * @brief Handle command line arguments and call c_mean to compute the mean.
 * @note Reads grades from a file or stdin and calls the c_mean function to 
 * compute the mean. If an input file is specified by the user, the program
 * reads from the file. Otherwise, it reads from stdin.
 * 
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return exit code.
*/
int main(int argc, char** argv){
    FILE *fp;
    // handle args
    if (argc == 1 || !strcmp(argv[1], "-")){
        fp = stdin;
    } else if (argc > EXPECTED_ARGS){
        fprintf(stderr, "Error: too many arguments\n");
        return 1;
    } else {
        fp = fopen(argv[1], "r");
    }

    if (fp == NULL){
        fprintf(stderr, "Error: cannot open file %s\n" , argv[1]);
        return 1;
    }

    int err = c_mean(fp);
    fclose(fp);

    return err;
}

/**
 * @brief Computes the mean of the grades in the given file stream.
 * @note This function also handles invalid input and prints error messages 
 * to stderr.
 * 
 * @param fp The file pointer to read the grades from.
 * @return Exit code.
*/
int c_mean(FILE* fp){
    int err = 0;
    int grade = 0;
    int line_number = 0;
    int valid_lines = 0;
    double average = 0;

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
        
        average += grade;
        line_number++;
        valid_lines++;
    }

    fprintf(stdout, "%.2lf\n" , average/valid_lines);
    return 0;
}
