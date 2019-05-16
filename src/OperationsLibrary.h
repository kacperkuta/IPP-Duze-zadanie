//
// Created by baka475 on 08.05.19.
//

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "map.h"
#include "CreatingRouteOperation.h"

#ifndef DROGI_OPERATIONSLIBRARY_H
#define DROGI_OPERATIONSLIBRARY_H

/** @brief Funkcja wywoływana w przypadku, gdy pierwsze polecenie w linii przed średnikiem to addRoad.
 * Sprawdza poprawność danych wejściowych. Jeżeli gdzieś wystąpi błąd wczytuje aż do napotkania znaku końca linii lub końca pliku.
 * Jeżeli wszystkie dane są poprawne podejmuje próbę wywołania polecenia addRoad.
 * @param map
 * @return @p false jeżeli dane wejściowe są niepoprawne lub wystąpi problem z alokacją pamięci. Wynik działania funkcji @ref addRoad wpp.
 */
bool addRoadCall (Map * map);

/** @brief Funkcja wywoływana w przypadku, gdy pierwsze polecenie w linii przed średnikiem to repairRoad.
 * Sprawdza poprawność danych wejściowych. Jeżeli gdzieś wystąpi błąd wczytuje aż do napotkania znaku końca linii lub końca pliku.
 * Jeżeli wszystkie dane są poprawne podejmuje próbę wywołania polecenia repairRoad.
 * @param map
 * @return @p false jeżeli dane wejściowe są niepoprawne lub wystąpi problem z alokacją pamięci. Wynik działania funkcji @ref repairRoad wpp.
*/
 bool repairRoadCall (Map * map);

/** @brief Funkcja wywoływana w przypadku, gdy pierwsze polecenie w linii przed średnikiem to getRouteDescription.
 * Sprawdza poprawność danych wejściowych. Jeżeli gdzieś wystąpi błąd wczytuje aż do napotkania znaku końca linii lub końca pliku.
 * Jeżeli wszystkie dane są poprawne podejmuje próbę wywołania polecenia getRouteDescription.
 * @param map
 * @return @p false jeżeli dane wejściowe są niepoprawne lub wystąpi problem z alokacją pamięci. Wynik działania funkcji @ref getRouteDescription wpp.
*/
bool getRouteDescriptionCall (Map * map);

/** @brief Realizuje jedną linię wejścia.
 * Analizuje polecenie i wywołuje odpowiednią funkcję. W przypadku błędu, niepoprawnego polecenia, braku wolnej pamięci, wypisuje ERROR i numer linii
 * na standardowe wyjście diagnostyczne.
 * @param map Wskaźnik na mapę.
 * @param lineNumber Numer linii wejścia.
 */
void realizeLine (Map * map, int lineNumber);

/** @brief Wywołuje funkcję @ref realizeLine dla wszystkich linii pliku aż do napotkania znaku końca pliku.
 * Ignoruje linie zaczynające się od znaku '#' oraz puste linie.
 * @param map Wskaźnik na mapę.
 */
void realizeAllLines (Map * map);

#endif //DROGI_OPERATIONSLIBRARY_H
