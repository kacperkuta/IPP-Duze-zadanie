//
// Created by baka475 on 08.05.19.
//

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "map.h"
#include "ReadLibrary.h"
#include "CreatingRouteOperation.h"

#ifndef DROGI_OPERATIONSLIBRARY_H
#define DROGI_OPERATIONSLIBRARY_H

bool addRoadCall (Map * map) {
    if (!checkSemicolon()) {
        return false;
    }

    const char * city1 = readToSemicolon();
    if (!checkSemicolon()) {
        freeIfNotNULL(city1);
        return false;
    }

    const char * city2 = readToSemicolon();
    if (!checkSemicolon()) {
        freeIfNotNULL(city1);
        freeIfNotNULL(city2);
        return false;
    }

    const char * length = readToSemicolon();
    if (!checkSemicolon()) {
        freeIfNotNULL(city1);
        freeIfNotNULL(city2);
        freeIfNotNULL(length);
        return false;
    }

    const char * builtYear = readToSemicolon();
    if (!checkEOL()) {
        freeIfNotNULL(city1);
        freeIfNotNULL(city2);
        freeIfNotNULL(length);
        freeIfNotNULL(builtYear);
        return false;
    }
    if (!isNumber(length) || !isNumber(builtYear) ||
        !strcmp("", city1) || !strcmp("", city2)) {
        freeIfNotNULL(city1);
        freeIfNotNULL(city2);
        freeIfNotNULL(length);
        freeIfNotNULL(builtYear);
        checkEOL();
        return false;
    }

    bool success = true;
    int length1 = convertStringToInteger(length);
    if (length1 <= 0)
        success = false;
    int builtYear1 = convertStringToInteger(builtYear);
    if (length1 > 0)
        success = addRoad(map, city1, city2, (unsigned)length1, builtYear1);
    freeIfNotNULL(city1);
    freeIfNotNULL(city2);
    freeIfNotNULL(length);
    freeIfNotNULL(builtYear);
    return success;
}

bool repairRoadCall (Map * map) {
    if (!checkSemicolon()) {
        return false;
    }

    const char * city1 = readToSemicolon();
    if (!checkSemicolon()) {
        freeIfNotNULL(city1);
        return false;
    }

    const char * city2 = readToSemicolon();
    if (!checkSemicolon()) {
        freeIfNotNULL(city1);
        freeIfNotNULL(city2);
        return false;
    }

    const char * repairYear = readToSemicolon();

    if (!checkEOL()) {
        freeIfNotNULL(city1);
        freeIfNotNULL(city2);
        freeIfNotNULL(repairYear);
        return false;
    }
    if (!isNumber(repairYear) || !strcmp("", city1) || !strcmp("", city2)) {
        checkEOL();
        freeIfNotNULL(city1);
        freeIfNotNULL(city2);
        freeIfNotNULL(repairYear);
        return false;
    }

    int repairYear1 = convertStringToInteger(repairYear);
    return repairRoad(map, city1, city2, repairYear1);
}

bool getRouteDescriptionCall (Map * map) {
    if (!checkSemicolon()) {
        return false;
    }
    const char *id = readToSemicolon();
    if (!checkEOL()) {
        freeIfNotNULL(id);
        return false;
    }
    if (!isNumber(id)) {
        checkEOL();
        freeIfNotNULL(id);
        return false;
    }
    int routeId = convertStringToInteger(id);
    if (routeId < 0) {
        freeIfNotNULL(id);
        return false;
    }
    const char * description = getRouteDescription(map, (unsigned)routeId);
    if (description) {
        printf("%s\n", description);
    }
    freeIfNotNULL(description);
    freeIfNotNULL(id);
    return description != NULL;
}

void realizeLine (Map * map, int lineNumber) {
    const char * instr = readToSemicolon();
    int type = commandType(instr);
    if (type == INCORRECT_COMMAND) {
        checkEOL();
        freeIfNotNULL(instr);
        fprintf(stderr, "ERROR %d\n", lineNumber);
    } else if (type == ADD) {
        if (!addRoadCall(map))
            fprintf(stderr, "ERROR %d\n", lineNumber);
    } else if (type == REPAIR) {
        if (!repairRoadCall(map))
            fprintf(stderr, "ERROR %d\n", lineNumber);
    } else if (type == DESCRIPTION) {
        if (!getRouteDescriptionCall(map))
            fprintf(stderr, "ERROR %d\n", lineNumber);
    } else {
        if (!routeCreation(map, (unsigned)type))
            fprintf(stderr, "ERROR %d\n", lineNumber);
    }
}

void realizeAllLines (Map * map) {
    int sign = getchar();
    int i = 1;
    while (sign != EOF) {
        ungetc(sign, stdin);
        if (sign == '#' || sign == '\n')
            checkEOL();
        else
            realizeLine(map, i);
        i++;
        sign = getchar();
    }
}

#endif //DROGI_OPERATIONSLIBRARY_H
