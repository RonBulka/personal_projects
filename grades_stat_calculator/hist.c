#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {MAX_GRADE = 100, MIN_GRADE = 0, DEFAULT_BIN = 10, MAX_ARGS = 4};

int parsed_file(int argc, char** argv);
int c_hist(int *bins);
int is_digit(char* string);

static FILE *fp;
static int num_bins = DEFAULT_BIN;

/**
 * @brief Compute the histogram of grades.
 * @param argc The number of command line arguments.
 * @param argv Array of command line arguments.
 * @return Exit code.
*/
int main(int argc, char** argv){
    int err = 0;
    err = parsed_file(argc, argv);
    if (err != 0){
        return 1;
    }
    
    int* bins = (int*)calloc(num_bins, sizeof(int));
    if (bins == NULL){
        fprintf(stderr, "Error: cannot allocate memory\n");
        return 1;
    }

    err = c_hist(bins);
    fclose(fp);
    free(bins);

    return err;
}

/**
 * @brief Parse command line arguments and open the input file.
 * @param argc The number of command line arguments.
 * @param argv Array of command line arguments.
 * @return Exit code.
 */
int parsed_file(int argc, char** argv){
    fp = stdin;
    if (argc > MAX_ARGS){
        fprintf(stderr, "Error: too many arguments\n");
        return 1;
    }
    for (int i = 1; i < argc; i++){
        if (!strcmp(argv[i], "-")){
            fp = stdin;
        } else if (!strcmp(argv[i], "-n_bins")){
            if (i < argc && is_digit(argv[i+1])){
                num_bins = atoi(argv[i+1]);
                i++;
            }
        } else {
            fp = fopen(argv[i], "r");
            if (fp == NULL){
                fprintf(stderr, "Error: cannot open file %s\n", argv[i]);
                return 1;
            }
        }
    }
    return 0;
}

/**
 * @brief Compute the histogram of grades
 * @param bins Array where to store the histogram
 * @return int 0 if success, 1 otherwise
 */
int c_hist(int *bins){
    int err = 0;
    int grade = 0;
    int line_number = 0;
    int n = 0;
    double pace = MAX_GRADE / num_bins;
    pace = ((int) pace == 0) ? 1 : pace;

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
        
        if (grade < MAX_GRADE){
            n = grade / pace;
        }
        else{
            n = num_bins - 1;
        }
        if (n >= num_bins){
            fprintf(stderr, "Error: bin num invalid. Tried bin num: %d\n", 
                    n + 1);
            return 1;
        }
        bins[n]++;
        line_number++;
    }

    for (int i = 0; i < num_bins; i++){
        printf("%.0f-%.0f\t%d\n", pace * i,
                                  (i < num_bins - 1) ? 
                                  pace * (i + 1) - 1 : MAX_GRADE, 
                                  bins[i]);
    }
    return 0;
}

/**
 * @brief Check if a string is a number.
 * 
 * @param string pointer to the string we want to check.
 * @return int 1 if the string is a number, 0 otherwise.
 */
int is_digit(char* string){
    char curr_char;
    while(*string){
        curr_char = *string++;
        if (curr_char < '0' || curr_char > '9'){
            return 0;
        }
    }
    return 1;
}