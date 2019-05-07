//
// Created by baka475 on 28.04.19.
//
#include <limits.h>
#include <stdio.h>
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

ListOfPaths * findAllEqualPathsForExtend (Map * map, int length, ListOfCities * city, ListOfCities * finalCity, City * previous, unsigned routeId, dijkstra ** distanceTable) {
    if (length == 0 && city == finalCity) {
        return endOfPath(previous);
    } else if (length > 0) {
        City * neighbours = city -> neighbour;
        ListOfPaths * allPaths = createEmpty();
        while (neighbours) {
            bool case1 = typeOfRouteElement(map, routeId, neighbours -> mainListRep) == OUT_OF_ROUTE &&
                          (!previous || (previous -> oppositeWay) -> cityID != neighbours -> cityID);
            bool case2 = neighbours -> cityID == finalCity -> cityID && !isInRoute(neighbours, routeId) && !(isInRoute(neighbours->oppositeWay, routeId));

            bool stepCondition = (length - neighbours->distance == distanceTable[neighbours->cityID]->minDistance);
            if (stepCondition && (case1 || case2)) {
                unsigned distance = neighbours->distance;
                ListOfPaths *newElement = findAllEqualPathsForExtend(map, length - distance, neighbours->mainListRep, finalCity,
                                                                     neighbours, routeId, distanceTable);
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
void shortestDistanceForExtend (Map * map, ListOfCities * origin, ListOfCities * destination, unsigned routeId, dijkstra ** visitedTable) {
    ListOfCities * isVisited = origin;
    priorityQueue * queue = newQueue();

    while (isVisited) {
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
}
Path * bestPathForExtend (Map * map, ListOfCities * origin, ListOfCities * destination, unsigned routeId)  {
    int tableLength = amountOfCities(map);
    dijkstra * distanceTable[tableLength];
    if (initializeVisitedTable(tableLength, distanceTable, origin) == MEMORY_PROBLEM)
        return false;
    shortestDistanceForExtend(map, origin, destination, routeId, distanceTable);

    int distance = distanceTable[destination->cityID]->minDistance;
    if (distance == INT_MAX) {
        freeDijkstraTable(tableLength, distanceTable);
        return NULL;
    }
    ListOfPaths * paths = findAllEqualPathsForExtend(map, distance, destination, origin, NULL, routeId, distanceTable);
    if (!paths)
        return NULL;
    Path * new =  chooseBest(paths);
    freePaths(paths, new);
    freeDijkstraTable(tableLength, distanceTable);
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
    if (!best)
        return false;
    if (!continuePath(best, routeId))
        return false;
    return true;
}
bool caseEqualStartAndFinish (Map * map, int distanceFromStart, ListOfCities * start, City * finish, ListOfCities * city,
                              unsigned routeId, dijkstra ** distanceTableFromStart, dijkstra ** distanceTableFromFinish) {
    ListOfPaths * fromStart = findAllEqualPathsForExtend(map, distanceFromStart, city, start, NULL, routeId, distanceTableFromStart);
    ListOfPaths * fromFinish = findAllEqualPathsForExtend(map, distanceFromStart, city, finish->mainListRep, NULL, routeId, distanceTableFromFinish);

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
    int tableLength = amountOfCities(map);
    dijkstra * distanceTableFromFinish[tableLength];
    if (initializeVisitedTable(tableLength, distanceTableFromFinish, finish->mainListRep) == MEMORY_PROBLEM)
        return false;
    dijkstra * distanceTableFromStart[tableLength];
    if (initializeVisitedTable(tableLength, distanceTableFromStart, start) == MEMORY_PROBLEM)
        return false;
    shortestDistanceForExtend(map, finish -> mainListRep, cityMainRep, routeId, distanceTableFromFinish);
    shortestDistanceForExtend(map, start, cityMainRep, routeId, distanceTableFromStart);

    int distanceFromStart = distanceTableFromStart[cityMainRep->cityID]->minDistance;
    int distanceFromFinish = distanceTableFromFinish[cityMainRep->cityID]->minDistance;

    bool test;

    if (distanceFromStart < distanceFromFinish) {
        test = caseExtendFromStart(map, start, cityMainRep, routeId);
    } else if (distanceFromStart > distanceFromFinish) {
        test = caseExtendFromFinish(map, finish -> mainListRep, cityMainRep, routeId);
    } else if (distanceFromFinish != INT_MAX) {
        test = caseEqualStartAndFinish(map, distanceFromStart, start, finish, cityMainRep, routeId, distanceTableFromStart, distanceTableFromFinish);
    } else {
        test = false;
    }
    freeDijkstraTable(tableLength, distanceTableFromFinish);
    freeDijkstraTable(tableLength, distanceTableFromStart);
    return test;
}





