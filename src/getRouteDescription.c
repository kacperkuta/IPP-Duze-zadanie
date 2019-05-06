//
// Created by baka475 on 28.04.19.
//

#include "getRouteDescription.h"
#include <stdlib.h>
#include <string.h>
bool checkMemory(char ** description, int * length, int expectedLength) {
    if (*length < expectedLength) {
        (*length) = expectedLength * 2;
        (*description) = realloc((*description), (size_t)(*length));
        if (!(*description))
            return false;
    }
    return true;
}
int howManyLetters(unsigned ID) {
    int multi = 10;
    int howMany = 1;
    while (ID/multi != 0) {
        howMany++;
        ID /= multi;
    }
    return howMany;
}
void addIntToString(char * string, unsigned id, int * descLength) {
    int i = howManyLetters(id);
    (*descLength) += i + 1;
    for (int j = 0; j < i; j++) {
        string[i-j-1] = (char)(id % 10) + (char)('0');
        id /= 10;
    }
    string[i] = ';';
    string[i+1] = '\0';
}
char * generateStringFromInt(int number) {
    int howLong = howManyLetters((unsigned)abs(number));
    if (number < 0)
        howLong++;
    char * string = malloc((size_t)howLong + START_SIZE);
    if (!string)
        return NULL;
    int id = 0;
    if (number < 0) {
        string[0] = '-';
        string[1] = '\0';
        char * t = generateStringFromInt(abs(number));
        char * toReturn =  strcat(string, t);
        free(t);
        return toReturn;
    }
    number = abs(number);
    for (int i = id; i < howLong + id; i++) {
        string[howLong - i - 1] = (char)(number % 10) + (char)('0');
        number /= 10;
    }
    string[id + howLong] = '\0';
    return string;
}
bool addCityToDescription(Map * map, City * city, char ** description, int * size, int * descLength) {
    int cityId = city->cityID;
    char * end = ";";

    char * distance = generateStringFromInt(city -> distance);
    if (!distance) {
        free(*description);
        return false;
    }
    int distLength = (int)strlen(distance);
    checkMemory(description, size, (*descLength) + distLength + 2*sizeof(char));
    (*description) = strcat(*description, distance);
    (*description) = strcat(*description, end);
    (*descLength) += distLength + 1;

    char * builtYear = generateStringFromInt(city -> buildYear);
    if (!builtYear) {
        free(*description);
        return false;
    }
    int yearLength = (int)strlen(builtYear);
    checkMemory(description, size, (*descLength) + yearLength + 2*sizeof(char));
    (*description) = strcat(*description, builtYear);
    (*description) = strcat(*description, end);
    (*descLength) += yearLength + 1;

    const char *name = (map -> citiesID)[cityId];
    int nameLength = (int)strlen(name);
    checkMemory(description, size, (*descLength) + nameLength + 2*sizeof(char));
    (*description) = strcat(*description, name);
    (*description) = strcat(*description, end);
    (*descLength) += nameLength + 1;

    free(distance);
    free(builtYear);
    return true;
}
const char * generateEmpty (char * old) {
    free(old);
    char * empty = malloc(sizeof(char));
    if (!empty)
        return NULL;
    empty[0] = '\0';
    return empty;
}
bool addFirstCity (Map * map, ListOfCities * city, char ** description, int * size, int * descLength) {
    const char * nameOfFirst = (map -> citiesID)[city -> cityID];
    int lengthOfFirst = (int)strlen(nameOfFirst);
    if (!checkMemory(description, size, (*descLength) + lengthOfFirst + START_SIZE))
        return false;
    (*description) = strcat((*description), nameOfFirst);
    (*description) = strcat((*description), ";");
    (*descLength) += lengthOfFirst + 1;
    return true;
}

