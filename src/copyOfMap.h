/** @file
 * Interfejs funkcji używanej do kopiowania mapy.
 * @author Kacper Kuta <k.kuta2@student.uw.edu.pl>
 * @date 8.04.2019
 */


#ifndef DROGI_COPYOFMAP_H
#define DROGI_COPYOFMAP_H
#include "map.h"
/** @brief Funkcja kopiuje mapę.
 * Nie jest kopiowana tablica identyfikatorów miast.
 * @param map Wskaźnik na mapę do skopiowania.
 * @return NULL w przypadku niezaalokowania pamięci.
 * Wskaźnik na kopię mapy jeśli alokacja przebiegła poprawnie.
 */
Map * copyOfMap (Map * map);
#endif //DROGI_COPYOFMAP_H
