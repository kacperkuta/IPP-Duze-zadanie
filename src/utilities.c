//
// Created by baka475 on 28.04.19.
//
#include "utilities.h"
#include "freeMemory.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

ListOfCities * findCity (Map * map, int cityID) {
    ListOfCities * first = map -> firstCity;
    while (first && first -> cityID != cityID) {
        first = first -> next;
    }
    return first;
}
int checkYear(Path * path) {
    if (!path)
        return INT_MIN;
    int year = INT_MAX;
    City * element;
    path = path -> next;
    while (path) {
        element = path -> city;
        if (element -> buildYear < year)
            year = element -> buildYear;
        path = path -> next;
    }
    return year;
}
City * getRoad(Map * map, int originCityID, int targetCityID) {
    ListOfCities * start = findCity(map, originCityID);
    City * neighbour = start -> neighbour;
    if (!neighbour)
        return NULL;
    while (neighbour -> next && neighbour -> cityID != targetCityID)
        neighbour = neighbour -> next;
    if (neighbour -> cityID == targetCityID)
        return neighbour;
    else
        return NULL;
}
int searchForCityID(const char * city, const char ** citiesID, int citiesIDLength) {
    int id = CITY_NOT_EXIST;
    for (int i = 0; i < citiesIDLength; i++) {
        if (strcmp(citiesID[i], city) == 0) {
            id = i;
        }
    }
    return id;
}
bool isInRoute(City * city, unsigned routeId) {
    listOfRouteIDs * first = city -> routes;
    while (first && first -> routeID != routeId) {
        first = first -> next;
    }
    return first != NULL;
}
bool addIDToCity (unsigned id, City * city) {
    listOfRouteIDs * new = malloc(sizeof(listOfRouteIDs));
    if (!new)
        return false;
    new -> routeID = id;
    new -> next = city -> routes;
    new -> firstCity = NULL;
    city -> routes = new;
    return true;
}
ListOfCities * getStartCity (Map * map, unsigned id) {
    listOfRouteIDs * start = map -> IDs;
    while (start && start -> routeID != id) {
        start = start -> next;
    }
    if (!start)
        return NULL;
    else
        return start -> firstCity;
}
City * getNextCityOfRoute (ListOfCities * origin, unsigned routeId, ListOfCities ** previousCity) {
    City * neighbours = origin -> neighbour;
    while (neighbours && (!(isInRoute(neighbours, routeId)) || neighbours -> mainListRep == *previousCity)) {
        neighbours = neighbours -> next;
    }
    if (neighbours) {
        (*previousCity) = origin;
        return neighbours;
    } else {
        return NULL;
    }
}
void repairMap(Map * original, Map * copy) {
    ListOfCities * city = original -> firstCity;
    while (city) {
        freeMemory(city -> neighbour);
        ListOfCities * toDelete = city;
        city = city -> next;
        free(toDelete);
    }
    freeListOfRouteIDs(original -> IDs);
    original -> firstCity = copy -> firstCity;
    original -> IDs = copy -> IDs;
    free(copy);
}
