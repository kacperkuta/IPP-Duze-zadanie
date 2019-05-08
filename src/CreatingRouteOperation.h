//
// Created by baka475 on 08.05.19.
//

#ifndef DROGI_CREATINGROUTEOPERATION_H
#define DROGI_CREATINGROUTEOPERATION_H

#include <stddef.h>
#include "AddRepair.h"
#include "structures.h"
#include "utilities.h"
#include "ReadLibrary.h"
#include "newRoute.h"

#define UPDATE_YEAR 100
#define ADD_ROAD 101
#define NONE 102

typedef struct Route {
    struct element * road;
    struct Route * next;
} Route;

typedef struct element {
    const char * city;
    unsigned distance;
    int buildYear;
} element;

/** @brief Wyszukuje odcinek dorogwy między miastem city1 i city2.
 * @param map Mapa w której szukamy
 * @param city1 Nazwa pierwszego miasta
 * @param city2 Nazwa drugiego miasta
 * @return Wskaźnik w przypadku odnalezienia. NULL wpp.
 */
City * findRoad (Map * map, const char * city1, const char * city2) {
    int id1 = searchForCityID(city1, map ->  citiesID, map -> citiesIDLength);
    int id2 = searchForCityID(city2, map ->  citiesID, map -> citiesIDLength);

    return getRoad(map, id1, id2);
}

/** @brief Czyta element nowej drogi krajowej.
 * W przypadku błędnej reprezentacji odcinka wczytuje linię do końca.
 * @return NULL w przypadku błędu alokacji pamięci, lub niepoprawnego przedstawienia odcinka. Wskaźnik wpp.
 */
element * readElement () {
    if (!checkSemicolon())
        return NULL;
    const char * distanceString = readToSemicolon();

    if (!checkSemicolon() || !isNumber(distanceString)) {
        freeIfNotNULL(distanceString);
        return NULL;
    }
    const char * buildYearString = readToSemicolon();
    if (!checkSemicolon() || !isNumber(buildYearString)) {
        freeIfNotNULL(distanceString);
        freeIfNotNULL(buildYearString);
        return NULL;
    }
    const char * city = readToSemicolon();
    int distance = convertStringToInteger(distanceString);
    int buildYear = convertStringToInteger(buildYearString);
    if (distance <= 0) {
        freeIfNotNULL(city);
        freeIfNotNULL(distanceString);
        freeIfNotNULL(buildYearString);
    }
    element * new = malloc(sizeof(element));
    if (!new)
        return new;
    new -> city = city;
    new -> distance = (unsigned)distance;
    new -> buildYear = buildYear;
    return new;
}

void freeMemoryOfRoute (Route * r) {
    while (r) {
        freeMemoryOfRoute(r->next);
        free(r->road);
        free(r);
    }
}

/** @brief Wczytuje nową drogę krajową.
 * @return NULL jeśli błąd pamięci lub polecenie niepoprawne. Wskaźnik wpp.
 */
Route * readRouteToCreate () {
    Route * first = malloc(sizeof(Route));
    Route * route = first;
    if (!first || !checkSemicolon())
        return NULL;
    element * start = malloc(sizeof(element));
    start -> city = readToSemicolon();
    first -> road = start;
    first -> next = NULL;
    if (!checkSemicolon()) {
        freeMemoryOfRoute(first);
        return NULL;
    }

    int sign = ';';
    while (sign != '\n') {
        ungetc(sign, stdin);
        element * el = readElement();
        route -> next = malloc(sizeof(route));

        if (!el || !(route->next)) {
            if (el)
                free(el);
            freeMemoryOfRoute(first);
            return NULL;
        }
        route = route -> next;
        route -> road = el;
        route -> next = NULL;
        sign = getchar();
    }
    return first;
}

bool checkIfRoadCorrect (Map * map, Route * previous, Route * next) {
    City * road = findRoad(map, (previous->road->city), (next->road->city));
    if (!road)
        return true;
    return  !((road->distance != (next->road->distance)) || (road->buildYear) > (next->road->buildYear) || (next->road->buildYear) == 0);
}

bool checkIfRoutePossible (Map * map, Route * r) {
    bool test = true;
    while (test && r && r -> next) {
        test = checkIfRoadCorrect(map, r, r->next);
        r = r->next;
    }
    return test;
}

int checkNecessaryActions (Map * map, Route * previous, Route * next) {
    City * road = findRoad(map, (previous->road->city), (next->road->city));
    if (!road)
        return ADD_ROAD;
    if (road -> buildYear < next->road->buildYear)
        return UPDATE_YEAR;
    return NONE;
}

bool takeActions (Map * map, Route * previous, Route * next, int routeId) {
    switch (checkNecessaryActions(map, previous, next)) {
        case ADD_ROAD:
            if (!addRoad(map, previous->road->city, next->road->city, next->road->distance, next->road->buildYear))
                return false;
            break;
        case UPDATE_YEAR:
            repairRoad(map, previous->road->city, next->road->city, next->road->buildYear);
            break;
        default:
            break;
    }
    City * city = findRoad(map, previous->road->city, next->road->city);
    bool test3 = addIDToCity((unsigned)routeId, city);
    bool test2 = addIDToCity((unsigned)routeId, city -> oppositeWay);
    if (!test3|| !test2) {
        return false;
    }
    return true;
}

bool routeCreation (Map * map, int routeId) {
    if (routeId >= 1000)
        return false;
    Route * r = readRouteToCreate();
    Route * copy = r;
    if (checkIfRoutePossible(map, r)) {
        while (r && r->next) {
            bool test = takeActions(map, r, r->next, routeId);
            if (!test){
                //na razie nic nie robimy, tu bedzie przywracanie mapy
            }
            r = r->next;
        }
        bool test2 = addIDToMap((unsigned)routeId, map, findRoad(map, (copy->next)->road->city, copy->road->city));
        return test2;
    }
    return false;
}

#endif //DROGI_CREATINGROUTEOPERATION_H
