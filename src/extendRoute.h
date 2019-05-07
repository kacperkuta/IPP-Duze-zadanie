/** @file
 * Interfejs klasy fuknkcji używanych przez extendRoute oraz removeRoad.
 * @author Kacper Kuta <k.kuta2@student.uw.edu.pl>
 * @date 8.04.2019
 */

#ifndef DROGI_EXTENDROUTE_H
#define DROGI_EXTENDROUTE_H
#include "structures.h"
#include "map.h"
/** @brief
 * Miasto jest miastem poczatkowym drogi krajowej.
 */
#define START_CITY 4
/** @brief
 * Miasto jest miastem końcowym drogi krajowej.
 */
#define FINAL_CITY 5
/** @brief
 * Miasto jest pośrednim drogi krajowej.
 */
#define INSIDE_CITY 6
/** @brief
 * Miasto nie należy do drogi krajowej.
 */
#define OUT_OF_ROUTE 7

/** @brief Znajduje ostatni odcinek drogowy należący do drogi krajowej.
 * @param map Wskaźnik na mapę.
 * @param routeId Numer drogi krajowej
 * @return NULL, jeżeli taka droga krajowa nie istnieje. Wskaźnik na odcinek drogowy wpp.
 */
City * getFinalCityOfRoute (Map * map, unsigned routeId);
/** @brief Przypisuje odcinkom drogowym scieżki @p path należenie do drogi krajowej.
 * @param best Wskaźnik na wybraną najlepszą ścieżkę.
 * @param routeId Numer drogi krajowej.
 * @return @p false jeżeli best był NULLem lub nie udało się zaalokować pamięci.
 * @p true w przypadku powodzenia.
 */
bool continuePath (Path * best, unsigned routeId);
/** @brief Zmienia pierwsze miasto danej drogi krajowej
 * @param map Wskaźnik na mapę.
 * @param first Wskaźnik na odcinek drogowy prowadzący do nowego pierwszego miasta.
 * @param routeId Numer drogi krajowej.
 */
void changeFirstCityOfRoute (Map * map, City * first, unsigned routeId);
/** @brief Sprawdza typ elementu w kontekście danej drogi krajowej.
 * Sprawdza, czy miasto @p city jest początkiem, końcem, miastem pośrednim, lub czy nie należy w oógle do drogi krajowej.
 * @param map Wskaźnik na mapę.
 * @param routeId Numer drogi krajowej.
 * @param city Wskaźnik na sprawdzane miasto.
 * @return OUT_OF_ROUTE w przypadku, gdy miasto nie należy do drogi krajowej.
 * START_CITY w przypadku gdy jest jej początkiem.
 * FINAL_CITY w przypadku gdy jest jej końcem.
 * INSIDE_CITY w przypadku gdy jest miastem pośrednim.
 */
int typeOfRouteElement (Map * map, unsigned routeId, ListOfCities * city);
/** @brief Zwraca ostatni odcinek drogowy ścieżki.
 * @param path Wskaźnik na ścieżkę.
 * @return NULL, jeżeli path jest NULLem, wskaźnik na ostatni odcinek drogowy wpp.
 */
City * finalElementOfPath (Path * path);
/** @brief Znajduje wszytkie poprawne ścieżki o danej długości.
 * Znajduje wszystkie ścieżki z city do finalCity o długości length, które potencjalnie mogą być użyte do wydłużenia bądź uzupełnienia drogi krajowej.
 * @param map Mapa, w której ścieżek szukamy.
 * @param length Długość pozostała do celu.
 * @param city Wskaźnik na miasto startowe.
 * @param finalCity Wskaźnik na miasto docelowe.
 * @param previous Wskaźnik na odcinek drogowy, którym przyszliśmy do miasta startowego.
 * NULL, jeżeli jesteśmy w pierwszym mieście.
 * @param routeId Numer rozszerzanej bądź usuwanej drogi krajowej.
 * @return Lista ścieżek, jeżeli uda się jakąkolwiek znaleźć,
 * pusta lista ścieżek, generowana przez @ref createEmpty(), jeżeli nie udało się znależć żadnej ścieżki,
 * NULL w przypadku problemu z pamięcią.
*/
ListOfPaths * findAllEqualPathsForExtend (Map * map, int length, ListOfCities * city, ListOfCities * finalCity, City * previous, unsigned routeId, dijkstra ** visitedTable);
/** @brief Algorytm dijkstry znajdujący najkrótszą ścieżkę między miastami.
 * Znajduje najkrótszą ścieżkę między city a finalCity, która może być użyta do wydłużenia drogi krajowej bądź jej uzupełnienia.
 * @param map Wskaźnik na mapę.
 * @param origin Wskaźnik na miasto, z którego startuejmy.
 * @param destination Wskaźnik na miasto, do którego zmierzamy.
 * @param routeId Numer drogi krajowej, którą naprawiamy bądź wydłużamy.
 * @return @p INT_MAX jeżeli nie istnieje taka ścieżka.
 * @p false w przypadku problemów z pamięcią.
 * Znalezioną najkrótszą odległość w przypadku powodzenia.
 */
void shortestDistanceForExtend (Map * map, ListOfCities * origin, ListOfCities * destination, unsigned routeId, dijkstra ** visitedTable);
/** @brief Znajduje najlepszą ścieżkę, którą można wykorzystać do rozszerzenia bądź uzupełnienia drogi krajowej.
 * @param map Wskaźnik na mapę.
 * @param origin Wskaźnik na miasto startowe.
 * @param destination Wskaźnik na miasto docelowe.
 * @param routeId Numer drogi krajowej.
 * @return Wskaźnik na najlepszą ścieżkę, jeżeli uda się znaleźć, NULL wpp.
 */
Path * bestPathForExtend (Map * map, ListOfCities * origin, ListOfCities * destination, unsigned routeId);
/** @brief Wykonuje to, co powinna robić funkcja @ref extendRoute. Zakłada, że droga krajowa istnieje, a podane miasto nie należy już do niej.
 * @param map Wskaźnik na mapę.
 * @param start Wskaźnik na maist startowe.
 * @param finish Ostatni odcinek drogowy danej drogi krajowej.
 * @param cityMainRep Wskaźnik na miasto, do którego chcemy rozszerzyć drogę krajową.
 * @param routeId Numer drogi krajowej.
 * @return @p true w przypadku powodzenia.
 * @p false jeżeli nie uda się zaalokować pamięci, nie da się jednoznacznie albo wcale wydłużyć drogi krajowej.
 */
bool extendCorrectRoute (Map * map, ListOfCities * start, City * finish, ListOfCities * cityMainRep, unsigned routeId);

#endif //DROGI_EXTENDROUTE_H
