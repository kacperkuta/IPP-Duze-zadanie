/** @file
 * Interefejs klasy służącej do zwalniania pamięci.
 * @author Kacper Kuta <k.kuta2@student.uw.edu.pl>
 * @date 8.04.2019
 */
#ifndef DROGI_FREEMEMORY_H
#define DROGI_FREEMEMORY_H
#include "structures.h"
#include <stdlib.h>

/**@brief Zwalnia pamięć zaalokowaną na potrzeby algorytmu dijkstry w funkcji @ref shortestDistanceForExtend.
 * Nie zwalnia obszaru pamięci na który wskazuje @p table.
 * @param[in] length długość przekazanej tablicy
 * @param[in] table wskaźnik na tablicę struktur @ref dijkstra
 */
void freeDijkstraTable(int length, dijkstra ** table);
/** @brief Zwalnia pamięć wskazywaną przez wszystkie elementy ścieżki @p path.
 * Jeżeli @p path jest NULLem nic nie robi.
 * @param[in] path Wskaźnik na pierwszy element ścieżki.
 */
void deletePath (Path * path);
/** @brief Zwalnia pamięć całej listy ścieżek.
 * Nie zwalnia pamięci jednej ścieżki, na którą wskazuje @p chosen.
 * @param[in] paths Wskaźnik na listę ścieżek.
 * @param[in] chosen Wskażnik na pierwszy element wybranej, nieusuwanej ścieżki.
 */
void freePaths(ListOfPaths * paths, Path * chosen);
/** @brief Zwalnia pamięć listy numerów dróg krajowych.
 * Jeżeli @p routes jest NULLem nic nie robi.
 * @param[in] routes Wskaźnik na listę numerów dróg krajowych.
 */
void freeListOfRouteIDs(listOfRouteIDs * routes);
/** @brief Zwalnia pamięć listy sąsiadów wskazywanej przez @p neighbour.
 * Zwalnia przez wywołanie @ref freeListOfRouteIDs pamięć listy dróg krajowych każdego z sąsiadów.
 * @param[in] neighbour Wskaźnik na listę sąsiadów.
 */
void freeMemory(City * neighbour);

#endif //DROGI_FREEMEMORY_H
