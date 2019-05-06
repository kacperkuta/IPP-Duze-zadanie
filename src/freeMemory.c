//
// Created by baka475 on 28.04.19.
//

#include "freeMemory.h"

void freeDijkstraTable(int length, dijkstra ** table) {
    for (int i = 0; i < length; i++) {
        free(table[i]);
    }
}

void deletePath (Path * path) {
    if (path) {
        deletePath(path -> next);
        free(path);
    }
}

void freePaths(ListOfPaths * paths, Path * chosen) {
    while (paths) {
        if (paths -> path != chosen) {
            deletePath(paths -> path);
        }
        ListOfPaths * next = paths -> next;
        free(paths);
        paths = next;
    }
}

void freeListOfRouteIDs(listOfRouteIDs * routes) {
    if (routes) {
        freeListOfRouteIDs(routes -> next);
        free(routes);
    }
}

void freeMemory(City * neighbour) {
    if (neighbour) {
        freeListOfRouteIDs(neighbour -> routes);
        freeMemory(neighbour -> next);
        free(neighbour);
    }
}
