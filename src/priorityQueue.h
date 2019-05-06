/** @file
 * Interfejs kolejki priorytetowej.
 * @author Kacper Kuta <k.kuta2@student.uw.edu.pl>
 * @date 8.04.2019
 */


#include "structures.h"
#include <stdlib.h>
#include "map.h"

#ifndef DROGI_PRIORITYQUEUE_H
#define DROGI_PRIORITYQUEUE_H
/** @brief Tworzy nową kolejkę priorytetową.
 * @return Wskaźnik na nową kolejkę.
 */
priorityQueue * newQueue();
/** @brief Sprawdza, czy kolejka jest pusta
 * @param queue Sprawdzana kolejka.
 * @return @p true jezeli jest pusta, @p false wpp.
 */
int isEmpty (priorityQueue * queue);
/** @brief Zwraca miasto, będące pierwsze w kolejce.
 * @param queue Kolejka, z której element pobieramy.
 * @return NULL, jeżeli kolejka jest pusta. Wskaźnik na miasto wpp.
 */
ListOfCities * pop (priorityQueue * queue);
/** @brief Dodaje do kolejki miasto z wagą @p weight.
 * Jeżeli miasto już występuje w kolejce z większą wagą, to usuwa poprzednie wystapienie.
 * @param queue Kolejka, do której dodajemy.
 * @param element Wskaźnik na dodawane miasto.
 * @param weight Waga elementu.
 * @return @p true jeżeli uda się dodać, @p false wpp.
 */
bool push (priorityQueue * queue, ListOfCities * element, int weight);
/** @brief Zwalnia całą pamięć zajmowaną przez kolejkę.
 * @param queue Wskaźnik na kolejkę do usunęcia.
 */
void removeQueue(priorityQueue * queue);
#endif //DROGI_PRIORITYQUEUE_H
