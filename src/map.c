//Kacper Kuta
//Duze zadanie IPP 2019 cz.1
//implementacja map.h
//Data utworzenia: 8.04.2019
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "map.h"
#include "structures.h"
#include "getRouteDescription.h"
#include "AddRepair.h"
#include "removeRoad.h"
#include "extendRoute.h"
#include "freeMemory.h"
#include "utilities.h"
#include "newRoute.h"
#include "copyOfMap.h"

Map * newMap(void) {
    Map * newMap = malloc(sizeof(Map));
    if (!newMap)
        return NULL;
    newMap -> firstCity = NULL;
    newMap -> citiesID = NULL;
    newMap -> citiesIDLength = 0;
    newMap -> IDs = NULL;
    return newMap;
}
void deleteMap(Map * map) {
    if (!map)
        return;
    ListOfCities * city = map -> firstCity;
    while (city) {
        freeMemory(city -> neighbour);
        ListOfCities * toDelete = city;
        city = city -> next;
        free(toDelete);
    }
    freeListOfRouteIDs(map -> IDs);
    for (int i = 0; i < map -> citiesIDLength; i++) {
        free((char*)(map ->citiesID)[i]);
    }
    if (map -> citiesID)
        free(map->citiesID);
    free(map);
}

bool addRoad(Map * map, const char * city1, const char * city2, unsigned length, int buildYear) {
    if (!strcmp(city1, city2) || buildYear == 0 || length == 0)
        return false;
    int city1ID = searchForCityID(city1, map -> citiesID, map -> citiesIDLength);
    int city2ID = searchForCityID(city2, map -> citiesID, map -> citiesIDLength);

    City * city1Neighbour = createNeighbour(CITY_NOT_EXIST, length, buildYear, NULL);
    if (!city1Neighbour)
        return false;
    City * city2Neighbour = createNeighbour(CITY_NOT_EXIST, length, buildYear, NULL);
    if (!city2Neighbour) {
        free(city1Neighbour);
        return false;
    }
    if (city1ID != CITY_NOT_EXIST && city2ID != CITY_NOT_EXIST && getRoad(map, city1ID, city2ID)) {
        free (city1Neighbour);
        free(city2Neighbour);
        return false;
    }
    if (city1ID == CITY_NOT_EXIST) {
        bool city1Added = addCityToMap(map, city1);
        if (!city1Added) {
            free(city1Neighbour);
            free(city2Neighbour);
            return false;
        }
        city1ID = searchForCityID(city1, map -> citiesID, map -> citiesIDLength);
    }
    if (city2ID == CITY_NOT_EXIST) {
        bool city2Added = addCityToMap(map, city2);
        if (!city2Added) {
            removeFirstCity(map);
            free(city1Neighbour);
            free(city2Neighbour);
            return false;
        }
        city2ID = searchForCityID(city2, map -> citiesID, map -> citiesIDLength);
    }
    return createRoad(map, city1Neighbour, city2Neighbour, city1ID, city2ID);
}
bool repairRoad(Map * map, const char * city1, const char * city2, int repairYear) {
    int city1ID = searchForCityID(city1, map -> citiesID, map -> citiesIDLength);
    int city2ID = searchForCityID(city2, map -> citiesID, map -> citiesIDLength);

    if (city1ID == city2ID || city1ID == -1 || city2ID == -1 || repairYear == 0)
        return false;

    City * routeDescription = getRoad(map, city1ID, city2ID);

    if (!routeDescription || routeDescription -> buildYear > repairYear)
        return false;

    routeDescription -> buildYear = repairYear;
    (routeDescription -> oppositeWay) -> buildYear = repairYear;
    return true;
}
bool newRoute(Map * map, unsigned routeId,
              const char *city1, const char *city2) {
    int city1ID = searchForCityID(city1, map -> citiesID, map -> citiesIDLength);
    int city2ID = searchForCityID(city2, map -> citiesID, map -> citiesIDLength);
    if (routeId >= 1000 || routeId <= 0|| city1ID == -1 || city2ID == -1 || city1ID == city2ID || idExists(map, routeId))
        return false;
    Path * best = bestPath(map, findCity(map, city1ID), findCity(map, city2ID));
    if (!best)
        return false;
    return createRoute(routeId, map, best);
}
bool extendRoute (Map * map, unsigned routeId, const char * city) {
    ListOfCities * start = getStartCity(map, routeId);
    if (!start)
        return false;
    City * finish = getFinalCityOfRoute(map, routeId);
    int cityId = searchForCityID(city, map -> citiesID, map -> citiesIDLength);
    if (cityId == CITY_NOT_EXIST)
        return false;
    ListOfCities * cityMainRep = findCity(map, cityId);
    if (typeOfRouteElement(map, routeId, cityMainRep) != OUT_OF_ROUTE ) {
        return false;
    }
    return extendCorrectRoute(map, start, finish, cityMainRep, routeId);
}
bool removeRoad(Map * map, const char * city1, const char * city2) {
    int city1ID = searchForCityID(city1, map -> citiesID, map -> citiesIDLength);
    int city2ID = searchForCityID(city2, map -> citiesID, map -> citiesIDLength);
    if (city1ID == -1 || city2ID == -1)
        return false;
    City * road = getRoad(map, city1ID, city2ID);
    if (!road)
        return false;
    Map * copy = copyOfMap(map);
    bool test = repairAllRoutes(map, road);
    if (!test) {
        repairMap(map, copy);
        return false;
    }
    deleteMap(copy);
    removeFromMap(road);
    return true;
}
const char * getRouteDescription (Map * map, unsigned routeId) {
    int size = START_SIZE;
    char * description = malloc((size_t)size);
    int descLength = 0;
    ListOfCities * city = getStartCity(map, routeId);

    if (!city) {
        return generateEmpty(description);
    }

    addIntToString(description, routeId, &descLength);
    addFirstCity(map, city, &description, &size, &descLength);

    ListOfCities * previous = city;
    City * next = getNextCityOfRoute(city , routeId, &previous);

    while (next) {
        if (!addCityToDescription(map, next, &description, &size, &descLength))
            return NULL;
        next = getNextCityOfRoute(next -> mainListRep, routeId, &previous);
    }
    description[descLength - 1] = '\0';
    return description;
}






