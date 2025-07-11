#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LINE_LENGTH 1001
#define STRING_LENGTH 21

int startPosition = 2;

void load_data(char *array) {
    char line[LINE_LENGTH];
    array[0] = '\0';

    do {
        fgets(line, LINE_LENGTH, stdin);
        if (line[0] != '\n') {
            strcat(array, line);
        }
    } while (line[0] != '\n');
}

void remove_specific(char *array, char prefix) {
    int j = 0;
    int len = strlen(array);
    for (int i = 0; i < len; i++) {
        if ((prefix == 'd' && !isdigit(array[i])) || (prefix == 'p' && array[i] != '\n' && !ispunct(array[i])) || array[i] == '\n') {
            array[j++] = array[i];
        }
    }
    array[j] = '\0';
}

void find_words(char *text, char words[][STRING_LENGTH], int *wordCount, int lineNumbers[]) {
    int len = strlen(text);
    int word_start = -1;
    int lineCount = 1;

    for (int i = 0; i <= len; i++) {
        if (isalpha(text[i])) {
            if (word_start == -1) {
                word_start = i;
            }
        }
        if (i == len || !isalpha(text[i])) {
            if (word_start != -1) {
                strncpy(words[*wordCount], &text[word_start], i - word_start);
                words[*wordCount][i - word_start] = '\0';
                lineNumbers[*wordCount] = lineCount;
                (*wordCount)++;
                word_start = -1;
            }
        }
        if (text[i] == '\n') {
            lineCount++;
        }
    }
}

int isPrefix(const char *str, const char *prefix, int isCaseSensitive){
    size_t prefixLen = strlen(prefix);
    size_t strLen = strlen(str);

    if(prefixLen > strLen) return 0;

    for(size_t i=0; i<prefixLen;i++){
        char strChar = str[i];
        char prefixChar = prefix[i];
        if(!isCaseSensitive){
            strChar = tolower(strChar);
            prefixChar = tolower(prefixChar);
        }
        if(strChar != prefixChar){
            return 0;
        }
    }
    return 1;
}

void replace_in_array(char *array, const char *word, const char *newWord, size_t newWordLength) {
    char *pos = array;
    while ((pos = strstr(pos, word)) != NULL) {
        if ((pos == array || !isalpha(*(pos - 1))) && !isalpha(*(pos + strlen(word)))) {
            size_t size = strlen(array) - strlen(word) + newWordLength + 1;
            char *newArray = malloc(size);
            strncpy(newArray, array, pos - array);
            newArray[pos - array] = '\0';
            strncat(newArray, newWord, newWordLength);
            if(strlen(word) > newWordLength) {
                strncat(newArray, word + newWordLength, strlen(word) - newWordLength);
            }
            strcat(newArray, pos + strlen(word));
            strncpy(array, newArray, size);
            free(newArray);
            pos += newWordLength;
        } else {
            pos += strlen(word);
        }
    }
}

void replace(char *array, char words[][STRING_LENGTH], int wordCount, int argc, char *argv[], int isCaseSensitive) {
    char *replacement = argv[startPosition-1];
    int replacementLength = strlen(replacement);

    for(int i=0; i<wordCount; i++) {
        int wordLength = strlen(words[i]);
        if(argc == startPosition) {
            int replaceLength = wordLength < replacementLength ? wordLength : replacementLength;
            replace_in_array(array, words[i], replacement, replaceLength);
            replace_in_array(words[i], words[i], replacement, replaceLength);
        } else {
            for (int k = startPosition; k < argc; k++) {
                if (isPrefix(words[i], argv[k], isCaseSensitive)) {
                    int replaceLength = wordLength < replacementLength ? wordLength : replacementLength;
                    replace_in_array(array, words[i], replacement, replaceLength);
                    replace_in_array(words[i], words[i], replacement, replaceLength);
                }
            }
        }
    }
}

int largestNumberInArray(char words[][STRING_LENGTH], int *wordCount){
    int max_value = strlen(words[0]);
    for(int i=0;i<*wordCount;i++){
        if(strlen(words[i]) > max_value){
            max_value = strlen(words[i]);
        }
    }
    return max_value;
}

int main(int argc, char *argv[]) {
    char array[LINE_LENGTH];
    char tempArray[LINE_LENGTH];
    char words[LINE_LENGTH][STRING_LENGTH];
    char tempWords[LINE_LENGTH][STRING_LENGTH];
    int tempCount = 0;
    int wordCount = 0;
    int lineNumbers[LINE_LENGTH];
    int templineNumbers[LINE_LENGTH];
    int d = 0, p = 0, r = 0, R = 0, s = 0;
    int errorStatus = 0;

    for(int i=1; i<argc; i++){
        if(argv[i][0] == '-'){
            if(strcmp(argv[i], "-d") != 0 && strcmp(argv[i], "-p") != 0 && strcmp(argv[i], "-r") != 0 && strcmp(argv[i], "-R") != 0 && strcmp(argv[i], "-s") != 0){
                errorStatus = 1;
                printf("E1");
            }
            if((strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "-R") == 0)){
                if(i == argc-1){
                    errorStatus = 1;
                    printf("E2");
                } else if(argv[i+1][0] == '-'){
                    errorStatus = 1;
                    printf("E2");
                } else if(strlen(argv[i+1]) < 1 || strlen(argv[i+1]) > 20){
                    errorStatus = 1;
                    printf("E3");
                }
            }
        }
    }


    if(errorStatus == 0) {
        load_data(array);
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-d") == 0) {
                d = 1;
                startPosition++;
            }
            if (strcmp(argv[i], "-p") == 0) {
                p = 1;
                startPosition++;
            }
            if (strcmp(argv[i], "-r") == 0) {
                r = 1;
                startPosition++;
            }
            if (strcmp(argv[i], "-R") == 0) {
                R = 1;
                startPosition++;
            }
            if (strcmp(argv[i], "-s") == 0) {
                s = 1;
                startPosition++;
            }
        }


        if (d) {
            remove_specific(array, 'd');
        }

        if (p) {
            remove_specific(array, 'p');
        }

        strcpy(tempArray, array);
        find_words(array, words, &wordCount, lineNumbers);
        find_words(tempArray, tempWords, &tempCount, templineNumbers);

        if (r) {
            replace(array, words, wordCount, argc, argv, 1);
        }

        if (R) {
            replace(array, words, wordCount, argc, argv, 0);
        }

        if (s) {
            for (int i = 0; i < wordCount; i++) {
                printf("%d. %-*s:%-*s\n", lineNumbers[i], largestNumberInArray(tempWords, &wordCount), tempWords[i],largestNumberInArray(words, &wordCount), words[i]);
            }
        } else {
            printf("%s", array);
        }
    }

    return 0;
}