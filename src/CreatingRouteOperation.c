#include "CreatingRouteOperation.h"

#define UPDATE_YEAR 100
#define ADD_ROAD 101
#define NONE 102

/** @brief Struktura nowej drogi krajowej
 */
typedef struct Route {
    /** @brief Odcinek drogowy */
    struct element * road;
    /** @brief Wskaźnik na następny element drogi krajowej */
    struct Route * next;
} Route;

/** @brief Struktura elementu nowej drogi krajowej
 */
typedef struct element {
    /** @brief Wskaźnik na nazwę miasta */
    const char * city;
    /** @brief Długość odcinka drogowego */
    unsigned distance;
    /** @brief Rok budowy */
    int buildYear;
} element;

City * findRoad (Map * map, const char * city1, const char * city2) {
    int id1 = searchForCityID(city1, map ->  citiesID, map -> citiesIDLength);
    int id2 = searchForCityID(city2, map ->  citiesID, map -> citiesIDLength);
    return getRoad(map, id1, id2);
}

element * readElement () {
    if (!checkSemicolon())
        return NULL;
    const char * distanceString = readToSemicolon();
    if (!checkSemicolon()) {
        freeIfNotNULL(distanceString);
        return NULL;
    }
    const char * buildYearString = readToSemicolon();
    if (!checkSemicolon()) {
        freeIfNotNULL(distanceString);
        freeIfNotNULL(buildYearString);
        return NULL;
    }

    const char * city = readToSemicolon();
    if (!isNumber(distanceString) || !isNumber(buildYearString) || !city) {
        checkEOL();
        freeIfNotNULL(distanceString);
        freeIfNotNULL(buildYearString);
        freeIfNotNULL(city);
        return NULL;
    }

    int distance = convertStringToInteger(distanceString);
    int buildYear = convertStringToInteger(buildYearString);

    if (distance <= 0) {
        checkEOL();
        freeIfNotNULL(city);
        freeIfNotNULL(distanceString);
        freeIfNotNULL(buildYearString);
        return NULL;
    }
    element * new = malloc(sizeof(element));
    if (!new)
        return new;
    new -> city = city;
    new -> distance = (unsigned)distance;
    new -> buildYear = buildYear;
    freeIfNotNULL(distanceString);
    freeIfNotNULL(buildYearString);
    return new;
}

void freeMemoryOfRoute (Route * r) {
    if (r) {
        freeMemoryOfRoute(r->next);
        if (r->road) {
            free((void*)((r->road)->city));
            free((void *)(r->road));
        }
        free(r);
    }
}

Route * firstElementCreating () {
    Route * first = malloc(sizeof(Route));
    if (!first || !checkSemicolon()) {
        if (first)
            free(first);
        return NULL;
    }
    element * start = malloc(sizeof(element));
    if (!start) {
        free(first);
        return NULL;
    }
    start -> city = readToSemicolon();
    if (!(start->city)) {
        free(start);
        free(first);
        checkEOL();
        return NULL;
    }
    first -> road = start;
    first -> next = NULL;
    if (!checkSemicolon()) {
        freeMemoryOfRoute(first);
        return NULL;
    }
    return first;
}

Route * readRouteToCreate () {
    Route * first = firstElementCreating();
    if (!first)
        return NULL;
    Route * route = first;
    int sign = ';';
    while (sign != '\n') {
        ungetc(sign, stdin);
        element * el = readElement();
        if (!el) {
            freeMemoryOfRoute(first);
            return NULL;
        }
        route -> next = malloc(sizeof(Route));
        if (!(route -> next)) {
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
    if (next->road->distance <= 0 || next->road->buildYear == 0 || !strcmp(previous->road->city, next->road->city))
        return false;
    City * road = findRoad(map, (previous->road->city), (next->road->city));
    if (!road)
        return true;
    if (road->distance != (next->road->distance))
        return false;
    return  !((road->buildYear) > (next->road->buildYear) || (next->road->buildYear) == 0);
}

bool checkCrossingOfRoute (Route * r, Route * city) {
    while (r) {
        if (r != city && !strcmp((city->road->city), (r->road->city))) {
            return false;
        }
        r = r -> next;
    }
    return true;
}

bool checkIfRoutePossible (Map * map, Route * r) {
    Route * start = r;
    if (!r)
        return false;
    bool test = true;
    while (test && r && r -> next) {
        test = checkIfRoadCorrect(map, r, r->next);
        if (test)
            test = checkCrossingOfRoute(start, r);
        r = r->next;
    }
    if (test && r)
        test = checkCrossingOfRoute(start, r);
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
    if (routeId >= 1000) {
        checkEOL();
        return false;
    }
    Map * mapCopy = copyOfMap(map);
    Route * r = readRouteToCreate();
    Route * copy = r;
    if (checkIfRoutePossible(map, r) && !idExists(map, (unsigned)routeId) ) {
        while (r && r->next) {
            bool test = takeActions(map, r, r->next, routeId);
            if (!test){
                freeMemoryOfRoute(copy);
                repairMap(map, mapCopy);
                return false;
            }
            r = r->next;
        }
        bool test2 = addIDToMap((unsigned)routeId, map, findRoad(map, (copy->next)->road->city, copy->road->city));
        deleteMap(mapCopy);
        freeMemoryOfRoute(copy);
        return test2;
    }
    deleteMap(mapCopy);
    freeMemoryOfRoute(copy);
    return false;
}
