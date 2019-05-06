//
// Created by baka475 on 28.04.19.
//
#include <limits.h>
#include "extendRoute.h"
#include "utilities.h"
#include "freeMemory.h"
#include "newRoute.h"
#include "copyOfMap.h"
#include "priorityQueue.h"

City * getFinalCityOfRoute (Map * map, unsigned routeId) {
    ListOfCities * start = getStartCity(map, routeId);
    if (!start)
        return NULL;
    ListOfCities * previous = start;
    City * next = getNextCityOfRoute(start, routeId, &previous);
    City * before = next;
    while (next) {
        before = next;
        next = getNextCityOfRoute(before -> mainListRep, routeId, &previous);
    }
    return before;
}
bool continuePath (Path * best, unsigned routeId) {
    Path * beggining = best;
    if (!best)
        return false;
    City * element = (best -> next) -> city;
    best = best -> next;
    while (best) {
        bool test = addIDToCity(routeId, element);
        if (!test) {
            deletePath(beggining);
            return false;
        }
        test = addIDToCity(routeId, element -> oppositeWay);
        if (!test) {
            deletePath(beggining);
            return false;
        }
        best = best -> next;
        if (best)
            element = best -> city;
    }
    deletePath(beggining);
    return true;
}
void changeFirstCityOfRoute (Map * map, City * first, unsigned routeId) {
    listOfRouteIDs * start = map -> IDs;
    while (start && start -> routeID != routeId)
        start = start -> next;
    if (start)
        start -> firstCity = first -> mainListRep;
}

int quantityOfNeighboursWithRoute (unsigned routeId, ListOfCities * city) {
    int quantity = 0;
    City * neighbours = city -> neighbour;
    while (neighbours) {
        listOfRouteIDs * list = neighbours -> routes;
        while (list) {
            if (list -> routeID == routeId)
                quantity++;
            list = list -> next;
        }
        neighbours = neighbours -> next;
    }
    return quantity;
}
int typeOfRouteElement (Map * map, unsigned routeId, ListOfCities * city) {
    int quantity = quantityOfNeighboursWithRoute(routeId, city);
    if (quantity == 1) {
        ListOfCities * start = getStartCity(map, routeId);
        if (city -> cityID == start -> cityID) {
            return START_CITY;
        } else {
            return FINAL_CITY;
        }
    } else if (quantity > 0) {
        return INSIDE_CITY;
    } else {
        return OUT_OF_ROUTE;
    }
}
City * finalElementOfPath (Path * path) {
    if (path) {
        while (path -> next)
            path = path -> next;
        return path -> city;
    }
    return NULL;
}

ListOfPaths * findAllEqualPathsForExtend (Map * map, int length, ListOfCities * city, ListOfCities * finalCity, City * previous, unsigned routeId) {
    if (length == 0 && city == finalCity) {
        return endOfPath(previous);
    } else if (length > 0) {
        City * neighbours = city -> neighbour;
        ListOfPaths * allPaths = createEmpty();
        while (neighbours) {
            bool case1 = typeOfRouteElement(map, routeId, neighbours -> mainListRep) == OUT_OF_ROUTE &&
                          (!previous || (previous -> oppositeWay) -> cityID != neighbours -> cityID);
            bool case2 = neighbours -> cityID == finalCity -> cityID && !isInRoute(neighbours, routeId) && !(isInRoute(neighbours->oppositeWay, routeId));

            if (case1 || case2) {
                unsigned distance = neighbours->distance;
                ListOfPaths *newElement = findAllEqualPathsForExtend(map, length - distance, neighbours->mainListRep, finalCity,
                                                                     neighbours, routeId);
                allPaths = connectListOfPaths(newElement, allPaths);
            }
            neighbours = neighbours -> next;
        }
        if (!previous)
            previous = getRoad(map, (city->neighbour)->cityID, city -> cityID);
        bool test = addFirstToEachPath(previous, allPaths);
        if (test)
            return allPaths;
        else
            return NULL;
    } else {
        return createEmpty();
    }
}
int shortestDistanceForExtend (Map * map, ListOfCities * origin, ListOfCities * destination, unsigned routeId) {
    int tableLength = amountOfCities(map);
    dijkstra * visitedTable[tableLength];
    if (initializeVisitedTable(tableLength, visitedTable, origin) == MEMORY_PROBLEM)
        return false;
    ListOfCities * isVisited = origin;
    priorityQueue * queue = newQueue();

    while (visitedTable[destination -> cityID] -> visited == 0 && isVisited) {
        City *neighbours = isVisited->neighbour;
        while (neighbours) {
            if (typeOfRouteElement(map, routeId, neighbours -> mainListRep) == OUT_OF_ROUTE ||
                (neighbours -> cityID == destination -> cityID && !(isInRoute(neighbours, routeId)))) {
                if (!(visitedTable[neighbours->cityID]->visited)) {
                    push(queue, neighbours->mainListRep,
                         (visitedTable[isVisited->cityID] -> minDistance) + (neighbours->distance));
                    if (visitedTable[neighbours->cityID] -> minDistance >
                        (visitedTable[isVisited->cityID] -> minDistance) + (neighbours->distance))
                        visitedTable[neighbours->cityID] -> minDistance =
                                (visitedTable[isVisited->cityID] -> minDistance) + (neighbours->distance);
                }
            }
            neighbours = neighbours->next;
        }
        isVisited = pop(queue);
        if (isVisited != NULL) {
            visitedTable[isVisited->cityID]->visited = true;
        }
    }
    removeQueue(queue);
    int toReturn = visitedTable[destination -> cityID] -> minDistance;
    freeDijkstraTable(tableLength, visitedTable);
    return toReturn;
}
Path * bestPathForExtend (Map * map, ListOfCities * origin, ListOfCities * destination, unsigned routeId)  {
    int distance = shortestDistanceForExtend(map, origin, destination, routeId);
    if (distance == INT_MAX)
        return NULL;
    City * previous = NULL;
    ListOfPaths * paths = findAllEqualPathsForExtend(map, distance, origin, destination, previous, routeId);
    if (!paths)
        return NULL;
    Path * new =  chooseBest(paths);
    freePaths(paths, new);
    return new;
}

bool caseExtendFromStart (Map * map, ListOfCities * start, ListOfCities * finish, unsigned routeId) {
    Path * best = bestPathForExtend(map, start, finish, routeId);
    if (!continuePath(best, routeId))
        return false;
    changeFirstCityOfRoute(map, getRoad(map, (finish -> neighbour) -> cityID, finish -> cityID), routeId);
    return true;
}
bool caseExtendFromFinish (Map * map, ListOfCities * finish, ListOfCities * newEnd, unsigned routeId) {
    Path * best = bestPathForExtend(map, finish, newEnd, routeId);
    if (!continuePath(best, routeId))
        return false;
    return true;
}
bool caseEqualStartAndFinish (Map * map, int distanceFromStart, int distanceFromFinish, ListOfCities * start, City * finish, ListOfCities * city,
                              unsigned routeId) {
    ListOfPaths * fromStart = findAllEqualPathsForExtend(map, distanceFromStart, city, start, NULL, routeId);
    ListOfPaths * fromFinish = findAllEqualPathsForExtend(map, distanceFromFinish, finish -> mainListRep, city, NULL, routeId);
    if (!(fromFinish && fromStart))
        return false;
    fromStart = connectListOfPaths(fromStart, fromFinish);
    Path * best = chooseBest(fromStart);
    freePaths(fromStart, best);
    if (!best)
        return false;
    if (finalElementOfPath(best) -> cityID == start -> cityID) {
        City * startCity = getRoad(map, (city -> neighbour) -> cityID, city -> cityID);
        changeFirstCityOfRoute(map, startCity , routeId);
    }
    return continuePath(best, routeId);
}

bool extendCorrectRoute (Map * map, ListOfCities * start, City * finish, ListOfCities * cityMainRep, unsigned routeId) {
    int distanceFromFinish = shortestDistanceForExtend(map, finish -> mainListRep, cityMainRep, routeId);
    int distanceFromStart = shortestDistanceForExtend(map, start, cityMainRep, routeId);

    if (distanceFromStart < distanceFromFinish) {
        return caseExtendFromStart(map, start, cityMainRep, routeId);
    } else if (distanceFromStart > distanceFromFinish) {
        return caseExtendFromFinish(map, finish -> mainListRep, cityMainRep, routeId);
    } else if (distanceFromFinish != INT_MAX) {
        return caseEqualStartAndFinish(map, distanceFromStart, distanceFromFinish, start, finish, cityMainRep,routeId);
    } else {
        return false;
    }
}





