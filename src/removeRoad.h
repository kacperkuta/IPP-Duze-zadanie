/** @file
 * Interfejs klasy funkcji usuwających odcinek drogowy.
 * @author Kacper Kuta <k.kuta2@student.uw.edu.pl>
 * @date 8.04.2019
 */


#ifndef DROGI_REMOVEROAD_H
#define DROGI_REMOVEROAD_H
#include "structures.h"
#include "map.h"

/** @brief
 * Wystąpił problem z pamięcią.
 */
#define MEMORY_PROBLEM -5
/** @brief Usuwa odcinek drogowy.
 * @param road Odcinek drogowy do usunięcia.
 */
void removeFromMap (City * road);
/** @brief Naprawia wszystkie drogi krajowe przerwane usuwaniem odcinka roadToDelete.
 * @param map Wskaźnik na mapę, z której odcinek usuwamy.
 * @param roadToDelete Wskaźnik na odcinek drogowy do usunięcia.
 * @return Jeżeli udało się poprawić wszystkie drogi krajowe zwraca @p true.
 * Jeżeli wystapił błąd z alkoacją pamięci lub nie udało się jednoznacznie naprawić którejś drogi krajowej.
 */
bool repairAllRoutes (Map * map, City * roadToDelete);

#endif //DROGI_REMOVEROAD_H
