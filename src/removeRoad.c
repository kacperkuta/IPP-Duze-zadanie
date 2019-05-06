//
// Created by baka475 on 28.04.19.
//
#include "removeRoad.h"
#include <limits.h>
#include "extendRoute.h"
#include "freeMemory.h"
#include "map.h"
#include "utilities.h"
#include "newRoute.h"
#include "copyOfMap.h"
#include "priorityQueue.h"

listOfRouteIDs * removeFromListOfIDs (listOfRouteIDs * list, unsigned routeId) {
    listOfRouteIDs * previous = NULL;
    listOfRouteIDs * first = list;
    while (list && list -> routeID != routeId) {
        previous = list;
        list = list -> next;
    }
    if (!list)
        return first;
    if (previous) {
        previous -> next = list -> next;
    } else {
        first = list -> next;
    }
    free(list);
    return first;
}
bool removeOneElementRoute(Map * map, unsigned routeId, City * road) {
    Path * best = bestPathForExtend(map, road -> mainListRep, (road -> oppositeWay) -> mainListRep, routeId);
    if (!best)
        return false;
    continuePath(best, routeId);
    road -> routes = removeFromListOfIDs(road -> routes, routeId);
    (road -> oppositeWay) -> routes = removeFromListOfIDs((road -> oppositeWay) -> routes, routeId);
    return true;
}
void removeElementFromMap (City * road, ListOfCities * origin) {
    City * neighbours = origin -> neighbour;
    City * previous = NULL;
    while (neighbours  != road) {
        previous = neighbours;
        neighbours = neighbours -> next;
    }
    City * toDelete = neighbours;
    freeListOfRouteIDs(toDelete -> routes);
    if (!previous)
        origin -> neighbour = neighbours -> next;
    else
        previous -> next = neighbours -> next;

    free(toDelete);
}
void removeFromMap(City * road) {
    ListOfCities * origin = (road -> oppositeWay) -> mainListRep;
    ListOfCities * destination = road -> mainListRep;
    City * opp = road -> oppositeWay;
    removeElementFromMap(road, origin);
    removeElementFromMap(opp, destination);
}
bool deleteFromInsideOfRoute (Map * map, unsigned routeId, City * road) {
    int distance = shortestDistanceForExtend(map, road -> mainListRep, (road -> oppositeWay) -> mainListRep, routeId);
    if (distance == INT_MAX)
        return false;
    Path * best = bestPathForExtend(map, road -> mainListRep, (road -> oppositeWay) -> mainListRep, routeId);
    if (best) {
        road->routes = removeFromListOfIDs(road->routes, routeId);
        (road -> oppositeWay) -> routes = removeFromListOfIDs((road -> oppositeWay) -> routes, routeId);
    }
    return continuePath(best, routeId);
}
bool removeTailOfRoute (Map * map, City * road, unsigned routeId) {
    Path * best = bestPathForExtend(map, road -> mainListRep, (road -> oppositeWay) -> mainListRep, routeId);
    if (!best)
        return false;
    road -> routes = removeFromListOfIDs(road -> routes, routeId);
    (road -> oppositeWay) -> routes = removeFromListOfIDs((road -> oppositeWay) -> routes, routeId);
    return continuePath(best, routeId);
}
bool repairRoute(Map * map, unsigned routeId, City * road, int type1, int type2) {
    if ((type1 == FINAL_CITY || type1 == START_CITY) && (type2 == FINAL_CITY || type2 == START_CITY)) {
        return removeOneElementRoute(map, routeId, road);
    } else if (type1 == FINAL_CITY || type2 == FINAL_CITY || type1 == START_CITY || type2 == START_CITY) {
        return removeTailOfRoute(map, road, routeId);
    } else {
        return deleteFromInsideOfRoute(map, routeId, road);
    }
}
bool considerRoute(Map * map, unsigned routeId, City * road) {
    City * opposite = road -> oppositeWay;

    ListOfCities * city1MainRep = road -> mainListRep;
    ListOfCities * city2MainRep = opposite -> mainListRep;
    int typeOf1 = typeOfRouteElement(map, routeId, city1MainRep);
    int typeOf2 = typeOfRouteElement(map, routeId, city2MainRep);
    return repairRoute(map, routeId, road, typeOf1, typeOf2);
}
int howManyRoutes(unsigned ** table, listOfRouteIDs * list) {
    listOfRouteIDs * first = list;
    int i = 0;
    while (list) {
        i++;
        list = list -> next;
    }
    if (i > 0) {
        (*table) = malloc(i*sizeof(int));
        if (!(*table))
            return MEMORY_PROBLEM;
    }
    int j = 0;
    while (first) {
        (*table)[j] = first -> routeID;
        first = first -> next;
        j++;
    }
    return i;
}
bool repairAllRoutes (Map * map, City * roadToDelete) {
    unsigned * tableOfRoutes = NULL;
    int howMany = howManyRoutes(&tableOfRoutes, roadToDelete -> routes);
    if (howMany == MEMORY_PROBLEM)
        return false;
    for (int i = 0; i < howMany; i++) {
        bool test = considerRoute(map, tableOfRoutes[i], roadToDelete);
        if (!test) {
            free(tableOfRoutes);
            return false;
        }
    }
    if (tableOfRoutes)
        free(tableOfRoutes);
    return true;
}

