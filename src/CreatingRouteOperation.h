//
// Created by baka475 on 08.05.19.
//

#ifndef DROGI_CREATINGROUTEOPERATION_H
#define DROGI_CREATINGROUTEOPERATION_H

#include <stddef.h>
#include <stddef.h>
#include "AddRepair.h"
#include "structures.h"
#include "utilities.h"
#include "ReadLibrary.h"
#include "newRoute.h"
#include "copyOfMap.h"

typedef struct Route {
    struct element * road;
    struct Route * next;
} Route;

typedef struct element {
    const char * city;
    unsigned distance;
    int buildYear;
} element;

bool routeCreation (Map * map, int routeId);

#endif //DROGI_CREATINGROUTEOPERATION_H
