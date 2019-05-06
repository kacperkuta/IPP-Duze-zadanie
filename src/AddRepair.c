//
// Created by baka475 on 28.04.19.
//
#include "AddRepair.h"
#include <string.h>
#include <stdlib.h>
#include "utilities.h"

int createIDForCity(const char * city, const char *** citiesID, int * citiesIDLength) {
    (*citiesIDLength) += 1;
    const char ** newCitiesID = realloc((*citiesID), (*citiesIDLength) * sizeof(char*));
    char * name = malloc(strlen(city) + sizeof(char));
    if (!newCitiesID || !name)
        return CITY_NOT_EXIST;
    (*citiesID) = newCitiesID;
    name = strcpy(name, city);
    (*citiesID)[(*citiesIDLength) - 1] = name;
    return (*citiesIDLength) - 1;
}
bool addCityToMap (Map * map, const char * city) {
    ListOfCities * newCity = malloc(sizeof(ListOfCities));
    if (!newCity)
        return false;
    int newID = createIDForCity(city, &(map -> citiesID), &(map -> citiesIDLength));
    if (newID == CITY_NOT_EXIST) {
        free(newCity);
        return false;
    }
    newCity -> cityID = newID;
    newCity -> neighbour = NULL;
    newCity -> next = map -> firstCity;
    map -> firstCity = newCity;
    return true;
}
void removeFirstCity (Map * map) {
    ListOfCities * first = map -> firstCity;
    if (!first)
        return;
    map -> firstCity = first -> next;
    free(first);
}
bool addNeighbour (ListOfCities * city, City * neighbour) {
    City *first = city -> neighbour;
    if (!first) {
        city -> neighbour = neighbour;
        return true;
    }
    if (first -> cityID == neighbour -> cityID) {
        free(neighbour);
        return false;
    }
    while (first -> next) {
        first = first -> next;
        if (first -> cityID == neighbour -> cityID) {
            free(neighbour);
            return false;
        }
    }
    first -> next = neighbour;
    return true;
}
bool createRoad (Map * map, City * city1, City * city2, int city1ID, int city2ID) {
    ListOfCities * city1mainRep = findCity(map, city1ID);
    ListOfCities * city2mainRep = findCity(map, city2ID);

    city1 -> mainListRep = city1mainRep;
    city2 -> mainListRep = city2mainRep;
    city1 -> cityID = city1ID;
    city2 -> cityID = city2ID;
    city1 -> oppositeWay = city2;
    city2 -> oppositeWay = city1;

    bool success = addNeighbour(city1mainRep, city2);
    bool success2 = addNeighbour(city2mainRep, city1);

    return success && success2;
}
City * createNeighbour (int cityID, unsigned distance, int buildYear, ListOfCities * mainListRep) {
    City * result = malloc(sizeof(City));
    if (!result)
        return NULL;
    result -> cityID = cityID;
    result -> distance = distance;
    result -> buildYear = buildYear;
    result -> next = NULL;
    result -> mainListRep = mainListRep;
    result -> routes = NULL;
    return result;
}


