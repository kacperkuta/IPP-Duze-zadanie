//
// Created by baka475 on 28.04.19.
//
#include <limits.h>
#include "structures.h"
#include "utilities.h"
#include "priorityQueue.h"
#include "freeMemory.h"
#include "newRoute.h"
#include "copyOfMap.h"

ListOfPaths * connectListOfPaths (ListOfPaths * first, ListOfPaths * second) {
    if (!first || !second)
        return NULL;
    if (first -> path == NULL) {
        free(first);
        return second;
    }
    if (second -> path == NULL) {
        free(second);
        return first;
    }
    ListOfPaths * lastOfFirst = first -> last;
    lastOfFirst -> next = second;
    first -> last = second -> last;
    return first;
}
bool addFirstToEachPath (City * toAdd, ListOfPaths * paths) {
    while (paths && paths -> path) {
        Path * first = paths -> path;
        Path * add = malloc(sizeof(Path));
        if (!add)
            return false;
        paths -> path = add;
        add -> city = toAdd;
        add -> next = first;
        paths = paths -> next;
    }
    return true;
}

ListOfPaths * endOfPath (City * previous) {
    ListOfPaths * new = malloc(sizeof(ListOfPaths));
    Path * newPath = malloc(sizeof(Path));
    if (!new || !newPath)
        return NULL;
    newPath -> city = previous;
    newPath -> next = NULL;
    new -> path = newPath;
    new -> next = NULL;
    new -> last = new;
    return new;
}
ListOfPaths * createEmpty (void) {
    ListOfPaths * empty = malloc(sizeof(ListOfPaths));
    empty -> path = NULL;
    empty -> next = NULL;
    return empty;
}
ListOfPaths * findAllEqualPaths (Map * map, int length, ListOfCities * city, ListOfCities * finalCity, City * previous) {
    if (length == 0 && city == finalCity) {
        return endOfPath(previous);
    } else if (length > 0) {
        City * neighbours = city -> neighbour;
        ListOfPaths * allPaths = createEmpty();
        while (neighbours) {
            unsigned distance = neighbours -> distance;
            if (!previous || neighbours -> cityID != (previous -> oppositeWay) -> cityID) {
                ListOfPaths *newElement = findAllEqualPaths(map, length - distance, neighbours -> mainListRep, finalCity,
                                                            neighbours);
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

int amountOfCities(Map * map) {
    int amount = 0;
    ListOfCities * city = map -> firstCity;
    while(city) {
        amount++;
        city = city -> next;
    }
    return amount;
}
int initializeVisitedTable (int tableLength, dijkstra ** visitedTable, ListOfCities * origin) {
    for (int i = 0; i < tableLength; i++) {
        visitedTable[i] = malloc(sizeof(dijkstra));
        if (!(visitedTable[i])) {
            for (int j = 0; j < i; j++) {
                free(visitedTable[j]);
            }
            free(visitedTable);
            return MEMORY_PROBLEM;
        }
        visitedTable[i] -> visited = false;
        visitedTable[i] -> minDistance = INT_MAX;
    }
    visitedTable[origin -> cityID] -> minDistance = 0;
    visitedTable[origin -> cityID] -> visited = true;
    return true;
}
int shortestDistance (Map * map, ListOfCities * origin, ListOfCities * destination) {
    int tableLength = amountOfCities(map);
    dijkstra * visitedTable[tableLength];
    if (initializeVisitedTable(tableLength, visitedTable, origin) == MEMORY_PROBLEM)
        return false;
    ListOfCities * isVisited = origin;
    priorityQueue * queue = newQueue();

    while (isVisited && visitedTable[destination -> cityID] -> visited == 0) {
        City * neighbours = isVisited -> neighbour;
        while (neighbours) {
            if (!(visitedTable[neighbours->cityID] -> visited)) {
                push(queue, neighbours->mainListRep,
                     (visitedTable[isVisited->cityID] -> minDistance) + (neighbours->distance));
                if (visitedTable[neighbours->cityID] -> minDistance >
                    (visitedTable[isVisited->cityID] -> minDistance) + (neighbours->distance)) {
                    visitedTable[neighbours->cityID]->minDistance =
                            (visitedTable[isVisited->cityID]->minDistance) + (neighbours->distance);
                }
            }
            neighbours = neighbours -> next;
        }
        isVisited = pop(queue);
        if (isVisited)
            visitedTable[isVisited -> cityID] -> visited = true;
    }
    int toReturn = visitedTable[destination -> cityID] -> minDistance;
    freeDijkstraTable(tableLength, visitedTable);
    removeQueue(queue);
    return toReturn ;
}

Path * chooseBest(ListOfPaths * paths) {
    if (paths -> path == NULL)
        return NULL;
    int year = INT_MIN;
    Path * best = paths -> path;
    Path * considered = best;
    int unclear = 0;
    while (paths && considered) {
        int yearOfConsidered = checkYear(considered);
        if (yearOfConsidered > year) {
            best = considered;
            year = yearOfConsidered;
            unclear = 0;
        } else if (yearOfConsidered == year) {
            unclear = 1;
        }
        paths = paths -> next;
        if (paths)
            considered = paths -> path;
    }
    if (unclear)
        return NULL;
    return best;
}
Path * bestPath (Map * map, ListOfCities * origin, ListOfCities * destination) {
    int distance = shortestDistance(map, origin, destination);
    if (distance == INT_MAX)
        return NULL;
    City * previous = NULL;
    ListOfPaths * paths = findAllEqualPaths(map, distance, origin, destination, previous);
    if (!paths)
        return NULL;
    Path * new =  chooseBest(paths);
    freePaths(paths, new);
    return new;
}

bool idExists(Map * map, unsigned id) {
    listOfRouteIDs * element = map -> IDs;
    while (element) {
        if (element -> routeID == id)
            return true;
        element = element -> next;
    }
    return false;
}
bool addIDToMap (unsigned id, Map * map, City * first) {
    listOfRouteIDs * new = malloc(sizeof(listOfRouteIDs));
    if (!new)
        return false;
    new -> routeID = id;
    new -> next = map -> IDs;
    map -> IDs = new;
    new -> firstCity = first -> mainListRep;
    return true;
}
bool createRoute(unsigned routeID, Map * map, Path * path) {
    Map * copy = copyOfMap(map);
    City * first = path -> city;
    Path * beggining = path;
    City * element = (path -> next) -> city;
    path = path -> next;
    while (path) {
        bool test = addIDToCity(routeID, element);
        bool test2 = addIDToCity(routeID, element -> oppositeWay);
        if (!test|| !test2) {
            repairMap(map, copy);
            return false;
        }
        path = path -> next;
        if (path)
            element = path -> city;
    }
    deleteMap(copy);
    deletePath(beggining);
    return addIDToMap(routeID, map, first);
}


