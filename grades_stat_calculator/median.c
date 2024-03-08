#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {MAX_GRADE = 100, MIN_GRADE = 0, EXPECTED_ARGS = 2};

int c_median(FILE* fp);

/**
 * @brief Handle command line args and call c_median to compute the median.
 * @note reads grades from a file or stdin and calls the c_median function
 * to compute the median. If an input file is specified by the user, the
 * program reads from the file. Otherwise, it reads from stdin.
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
        fprintf(stderr, "Error: too many arguments\n");
        return 1;
    } else {
        fp = fopen(argv[1], "r");
    }

    if (fp == NULL){
        fprintf(stderr, "Error: cannot open file %s\n" , argv[1]);
        return 1;
    }

    
    int err = c_median(fp);
    return err;
}

/**
 * @brief Computes the median of the grades in the input file.
 * @note Prints the median grade to stdout.
 * 
 * @param fp Pointer to the input file.
 * @return Exit code.
 */
int c_median(FILE* fp){
    int err = 0;
    // MAX_GRADE + 1 possible grades
    int hist[MAX_GRADE + 1] = {0};
    int grade = 0;
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

        hist[grade]++;
        line_number++;
    }

    int median_idx = (line_number + 1) / 2;
    
    for (int i = 0; i < MAX_GRADE + 1; i++){
        median_idx -= hist[i];
        if (median_idx <= 0){
            fprintf(stdout, "%d", i);
            fclose(fp);
            break;
        }
    }

    return 0;
}
