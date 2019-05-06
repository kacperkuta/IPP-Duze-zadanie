//
// Created by baka475 on 28.04.19.
//
#include "copyOfMap.h"
#include <stdlib.h>
#include "utilities.h"
#include "structures.h"

listOfRouteIDs * copyOfIDs (Map * newMap, listOfRouteIDs * list) {
    if (!list)
        return NULL;
    listOfRouteIDs * first = malloc(sizeof(listOfRouteIDs));
    if (!first)
        return NULL;
    listOfRouteIDs * element = first;
    listOfRouteIDs * prev = NULL;
    while (list) {
        element -> routeID = list -> routeID;
        if (list -> firstCity)
            element -> firstCity = findCity(newMap, (list -> firstCity) -> cityID);
        else
            element -> firstCity = NULL;
        element -> next = malloc(sizeof(listOfRouteIDs));
        if (!(element -> next)) {
            return NULL;
        }
        prev = element;
        element = element -> next;
        list = list -> next;
    }
    free(element);
    if (prev)
        prev -> next = NULL;
    return first;
}
listOfRouteIDs * copyOfIDsWithoutFirstCity (listOfRouteIDs * list) {
    if (!list)
        return NULL;
    listOfRouteIDs * first = malloc(sizeof(listOfRouteIDs));
    listOfRouteIDs * element = first;
    listOfRouteIDs * prev = NULL;
    while (list) {
        element -> routeID = list -> routeID;
        element -> firstCity = NULL;
        element -> next = malloc(sizeof(listOfRouteIDs));
        prev = element;
        element = element -> next;
        list = list -> next;
    }
    free(element);
    if (prev)
        prev -> next = NULL;
    return first;
}
City * copyOfNeighbour (Map * newMap, City * neighbour) {
    City * new = malloc((sizeof(City)));
    if (!new)
        return NULL;
    new -> cityID = neighbour -> cityID;
    new -> distance = neighbour -> distance;
    new -> buildYear = neighbour -> buildYear;
    new -> routes = copyOfIDsWithoutFirstCity(neighbour -> routes);
    new -> oppositeWay = NULL;
    new -> mainListRep = findCity(newMap, new -> cityID);
    return new;
}
ListOfCities * copyOfListOfCities(ListOfCities * old) {
    if (!old)
        return NULL;
    ListOfCities * new = malloc(sizeof(ListOfCities));
    if (!new)
        return NULL;
    ListOfCities * first = new;
    ListOfCities * prev = NULL;
    while (old) {
        new -> cityID = old -> cityID;
        new -> neighbour = NULL;
        new -> next = malloc(sizeof(ListOfCities));
        if (!(new -> next))
            return NULL;
        prev = new;
        new = new -> next;
        old = old -> next;
    }
    free(new);
    if (prev)
        prev -> next = NULL;
    return first;
}
City * createAllNeighbours (Map * new, ListOfCities * oldOrigin) {
    City * element = malloc(sizeof(City));
    if (!element)
        return NULL;
    City * first = element;
    City * old = oldOrigin -> neighbour;
    while (old) {
        element -> next = copyOfNeighbour(new, old);
        if (!(element->next))
            return NULL;
        old = old -> next;
        element = element -> next;
    }
    element -> next = NULL;
    element = first -> next;
    free(first);
    return element;
}
City * findOppo (Map * map, int originId, int destId) {
    ListOfCities * start = map -> firstCity;
    while (start && start -> cityID != originId) {
        start = start -> next;
    }
    if (!start)
        return NULL;
    City * neighbours = start -> neighbour;
    while (neighbours && neighbours -> cityID != destId) {
        neighbours = neighbours -> next;
    }
    return neighbours;
}
void connectOpposites (Map * new) {
    ListOfCities * start = new ->firstCity;
    while (start) {
        City * neighbours = start  -> neighbour;
        while (neighbours) {
            if (neighbours -> oppositeWay == NULL) {
                City * oppo = findOppo(new, neighbours -> cityID, start -> cityID);
                neighbours -> oppositeWay = oppo;
                oppo -> oppositeWay = neighbours;
            }
            neighbours = neighbours -> next;
        }
        start = start -> next;
    }
}
Map * copyOfMap(Map * map) {
    Map * new = malloc(sizeof(Map));
    if (!new)
        return NULL;
    new -> firstCity = copyOfListOfCities(map -> firstCity);
    new -> citiesIDLength = 0;
    new -> citiesID = NULL;
    new -> IDs = copyOfIDs(new, map -> IDs);
    ListOfCities * city = new -> firstCity;
    ListOfCities * old = map -> firstCity;
    while (city) {
        city -> neighbour = createAllNeighbours(new, old);
        city = city -> next;
        old = old -> next;
    }
    connectOpposites(new);
    return new;
}