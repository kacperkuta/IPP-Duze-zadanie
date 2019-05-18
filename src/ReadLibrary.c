#include "ReadLibrary.h"


bool extendMemory (char ** description, int * length, int expectedLength) {
    if (*length < expectedLength) {
        (*length) = expectedLength * 2;
        (*description) = realloc((*description), (size_t)(*length));
        if (!(*description))
            return false;
    }
    return true;
}

const char * readToSemicolon () {
    char * inscription = NULL;
    int size = 0;
    int length = 1;
    int letter = getchar();
    while (letter != EOF && letter != '\n' && letter != ';') {
        length++;
        if (!(extendMemory(&inscription, &size, length))) {
            return NULL;
        }
        inscription[length-2] = (char)letter;
        letter = getchar();
    }
    ungetc(letter, stdin);
    if (inscription) {
        inscription[length - 1] = '\0';
    }
    return inscription;
}

bool isNumber (const char * inscription) {
    if (!inscription || strlen(inscription) == 0)
        return false;
    bool test = true;
    for (unsigned long i = (unsigned)(inscription[0] == '-'); i < strlen(inscription) && test; i++) {
        if ((i > 0 && !(inscription[i] >= '0' && inscription[i] <= '9' )) || (i == 0 && inscription[i] == '-'))
            test = false;
    }
    return test;
}

int convertStringToInteger (const char * string) {
    int negative = false;
    unsigned id = 0;
    int multi = 1;
    int number = 0;
    if (string[0] == '-') {
        negative = true;
        id = 1;
    }
    for (unsigned long i = strlen(string); i > id; i--) {
        number += multi*((int)string[i-1] - (int)'0');
        multi *= 10;
    }
    if (negative)
        number *= (-1);
    return number;
}

int commandType (const char * inscription) {
    if (!inscription)
        return INCORRECT_COMMAND;
    else if (isNumber(inscription))
        return convertStringToInteger(inscription) > 0 ? convertStringToInteger(inscription):INCORRECT_COMMAND;
    else if (!strcmp(inscription, "addRoad"))
        return ADD;
    else if (!strcmp(inscription, "repairRoad"))
        return REPAIR;
    else if (!strcmp(inscription, "getRouteDescription"))
        return DESCRIPTION;
    else
        return INCORRECT_COMMAND;
}

bool checkEOL () {
    int sign = getchar();
    bool test = true;
    if (sign != '\n')
        test = 0;
    while (sign != '\n' && sign != EOF)
        sign = getchar();
    return test;
}

bool checkSemicolon () {
    int letter = getchar();
    if (letter != ';') {
        ungetc(letter, stdin);
        checkEOL();
        return false;
    }
    return true;
}

void freeIfNotNULL (const char * pointer) {
    if (pointer)
        free((void *)pointer);
}


