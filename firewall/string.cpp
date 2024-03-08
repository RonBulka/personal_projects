#include "string.h"
#include <cstring>

String::String(): length(0){
    data = new char[length + 1];
    data[0] = '\0';
}


String::String(const String &str): length(str.length){
    data = new char[length + 1];
    strcpy(data, str.data);
}


String::String(const char *str){
    length = strlen(str);
    data = new char[length + 1];
    strcpy(data, str);
}


String::~String(){ delete[] data; }


String& String::operator=(const String &rhs){
    if (this != &rhs){
        delete[] this->data;
        this->length = rhs.length;
        this->data = new char[this->length + 1];
        strcpy(this->data, rhs.data);
    }
    return *this;
}


String& String::operator=(const char *str){
    if (equals(str)){
        return *this;
    }
    delete[] this->data;
    this->length = strlen(str);
    this->data = new char[length + 1];
    strcpy(this->data, str);
    return *this;
}


bool String::equals(const String &rhs) const {
    return strcmp(data, rhs.data) == 0;
}


bool String::equals(const char *rhs) const {
    return strcmp(data, rhs) == 0;
}


/*
* find the index of the first char which is one of the demimiters
* starting from data + start_index
*/ 
size_t findIndex(char* data,
                const char* delimiters,
                size_t len,
                int start_index = 0) {
    char* found = std::strstr(data + start_index, delimiters);
    if (found) {
        return found - data;
    }
    return len;
}

void String::split(const char *delimiters, String **output,
                   size_t *size) const {
    bool flag = true;
    size_t sub_length = 0;
    size_t num_of_words = 0;

    // output is NULL, user wants to know how many sub strings
    if (output == nullptr) {
        flag = false;
        *size = 0;
    }
    // output isn't NULL, user expects to get an array of sub strings
    else {
        *output = new String[*size];
    }

    for (size_t i = 0; i < length; i++) {
        size_t found_index = findIndex(data, delimiters, length, i);
        sub_length = found_index < length ? found_index - i : length - i;

        if (flag){
            // Allocate memory for sub-string and copy characters
            char *sub_string = new char[sub_length + 1];
            strncpy(sub_string, data + i, sub_length);
            sub_string[sub_length] = '\0';
            // Add sub-string to output
            (*output)[num_of_words] = String(sub_string);

            // Clean up
            delete[] sub_string;
        }
        else {
            (*size)++;
        }

        num_of_words++;
        i = found_index;
    }
}


int String::to_integer() const {
    int result = 0;
    int sign = 1;
    size_t i = 0;
    if (data[0] == '-') {
        sign = -1;
        i = 1;
    }
    for (; i < length; i++) {
        result *= 10;
        result += data[i] - '0';
    }
    return result*sign;
}


String String::trim() const {
    size_t start = 0;
    size_t end = length - 1;
    while (start < length && (data[start] == ' ' || data[start] == '\t' ||
                              data[start] == '\n')) {
        start++;
    }
    while (end > start && (data[end] == ' ' || data[end] == '\t' ||
                           data[end] == '\n')) {
        end--;
    }
    size_t new_length = end - start + 1;
    char *new_data = new char[new_length + 1];
    for (size_t i = 0; i < new_length; i++) {
        new_data[i] = data[start + i];
    }
    new_data[new_length] = '\0';

    String res = String(new_data);
    delete[] new_data;

    return res;
}