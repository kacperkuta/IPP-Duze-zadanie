/** @file
 * Interfejs funkcii używanej do tworzenia nowej drogi krajowej o przebiegu z pliku wejściowego.
 * @author Kacper Kuta <k.kuta2@student.uw.edu.pl>
 * @date 8.05.2019
 */

#ifndef DROGI_CREATINGROUTEOPERATION_H
#define DROGI_CREATINGROUTEOPERATION_H

#include <stddef.h>
#include <stddef.h>
#include "AddRepair.h"
#include "structures.h"
#include "utilities.h"
#include "ReadLibrary.h"
#include "newRoute.h"
#include "copyOfMap.h"

/** @brief Funkcja tworząca nową drogę krajową.
 * Tworzy nową drogę krajową o odcinkach podanych w poleceniu. Jeżeli to konieczne dodaje lub remontuje odcinki dróg.
 * Za niepoprawne uznaje: rok budowy równy 0, długośc odcinka drogowego niewiększą niż 0.
 * W razie niepowodzenia w dodaniu któregokolwiek odcinka lub niepoprawnego wejścia nie zmienia mapy.
 * @param map Wskaźnik na mapę.
 * @param routeId Numer drogi krajowej odczytany na początku linii polecenia.
 * @return @p true gdy opracja się powiedzie. @p false gdy polecenie było niepoprawne, niekompletne, lub wystąpił błąd w alokacji pamięci.
 */
bool routeCreation (Map * map, int routeId);

#endif //DROGI_CREATINGROUTEOPERATION_H
