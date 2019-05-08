//
// Created by baka475 on 07.05.19.
//

#ifndef DROGI_READLIBRARY_H
#define DROGI_READLIBRARY_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "map.h"

#define ADD -10
#define REPAIR -11
#define DESCRIPTION -12
#define INCORRECT_COMMAND -13

bool extendMemory (char ** description, int * length, int expectedLength) {
    if (*length < expectedLength) {
        (*length) = expectedLength * 2;
        (*description) = realloc((*description), (size_t)(*length));
        if (!(*description))
            return false;
    }
    return true;
}

/** @brief Wczytuje napis aż do natrafienia na średnik lub znak nowej linii '\n' lub EOF.
 * Pozostawia ostatni znak kończący wczytwanie na wejściu.
 * @return Wskaźnik na ten napis. NULL w przypadku problemu z alokacją pamięci lub jeśli nie było znaków do wczytania.
 */
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
    if (inscription)
        inscription[length-1] = '\0';
    return inscription;
}

/** @brief Sprawdza, czy napis jest liczbą
 * @param inscription wskaźnik na napis
 * @return @p true jeżeli napis jest liczbą
 * @p false wpp
 */
bool isNumber (const char * inscription) {
    if (strlen(inscription) == 0)
        return false;
    bool test = true;
    for (unsigned long i = 0; i < strlen(inscription) && test; i++) {
        if ((i > 0 && !(inscription[i] >= '0' && inscription[i] <= '9' )) || (i == 0 && inscription[i] == '-'))
            test = false;
    }
    return test;
}

/**@brief Konwertuje string będący liczbą całkowitą na inta.
 * @param string Wskaźnik na konwertowany napis.
 * @return Przekonwertowany napis na inta.
 */
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

/** @brief Informuje o otrzymanym poleceniu.
 * Sprawdza, jakie polecenie stoi przed pierwszym średnikiem w linii.
 * @param inscription Wskaźnik na napis odpowiadający poleceniu.
 * @return @p ADD jeżeli polecenie to addRoute.
 * @p REPAIR jeżeli polecenie to repairRoute
 * @p DESCRIPTION jeżeli polecenie to getRouteDescription.
 * liczbę dodatnią, jeżeli polecenie jest liczbą całkowitą dodatnią
 * @p INCORRECT_INSTRUCTION wpp
 */
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


#endif //DROGI_READLIBRARY_H
