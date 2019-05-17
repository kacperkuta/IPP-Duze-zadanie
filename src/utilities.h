/** @file
 * Interfejs klasy fuknkcji używanych przez wiele modułów programu.
 * @author Kacper Kuta <k.kuta2@student.uw.edu.pl>
 * @date 8.04.2019
 */

#ifndef DROGI_UTILITIES_H
#define DROGI_UTILITIES_H
#include "structures.h"
#include "map.h"
/** @brief
 * Miasto nie istnieje.
 */
#define CITY_NOT_EXIST -1

/** @brief Znajduje główną reprezentację miasta.
 * Znajduje główną reprezentację miasta o identyfikatorze @p cityID w mapie @p map.
 * @param[in] map Mapa w której szukamy miasta.
 * @param[in] cityID Identyfikator szukanego miasta.
 * @return Wskaźnik na szukaną reprezentację miasta, jeżeli takowa istnieje. NULL w przeciwnym przypadku.
 */
ListOfCities * findCity (Map * map, int cityID);
/** @brief Zwraca rok budowy lub ostatniej naprawy najstarszego odcinka w ścieżce @p path.
 * @param[in] path Sprawdzana ścieżka.
 * @return Rok budowy, lub @p INT_MIN jeżeli @p path jest NULLem.
 */
int checkYear(Path * path);
/** Znajduje odcinek drogowy między miastami.
 * @param[in] map Mapa, w której odcinka szukamy.
 * @param[in] originCityID Identyfikaotr miasta startowego.
 * @param[in] targetCityID Identyfikator miasta końcowego.
 * @return NULL jeżeli odcinek drogi nie istnieje lub nie istnieje któreś z miast o podanych identyfikatorach. Wskaźnik na odcinek drogowy w przeciwnym razie.
 */
City * getRoad(Map * map, int originCityID, int targetCityID);
/** @brief Znajduje identyfikator miasta.
 * @param city Wskaźnik na nazwę miasta, którego identyfikatora szukamy.
 * @param citiesID Tablica przechowująca identyfikatory.
 * @param citiesIDLength Długość tablicy przechowującej identyfikatory
 * @return Identyfikator miasta gdy ono istnieje, w przeciwnym razie -1.
 */
int searchForCityID(const char * city, const char ** citiesID, int citiesIDLength);
/** @brief Sprawdza, czy odcinek drogowy należy do podanej drogi krajowej.
 * Sprawdza czy odcinek drogowy @p city należy do drogi krajowej @p routeId.
 * @param city Wskaźnik na sprawdzany odcinek drogowy.Nie może być NULLem!
 * @param routeId Numer drogi krajowej.
 * @return @p true jesli należy, @p false jeśli nie należy.
 */
bool isInRoute(City * city, unsigned routeId);
/** @brief Przypisuje odcinkowi drogowemu drogę krajową.
 * Sprawia, że odcinek drogowy wskazywany przez @p city należy do drogi krajowej o numerze @p id.
 * @param id Numer drogi krajowej.
 * @param city Wskaźnik na odcinek drogowy, który przpisujemy do drogi krajowej.
 * @return @p true jeśli przypisanie się powiodło, @p false jeśli nie udało się zaalokować pamięci.
 */
bool addIDToCity (unsigned id, City * city);
/** @brief Funkcja szukająca pierwszego miasta danej drogi krajowej.
 * @param map Wskaźnik na mapę, w której szukamy pierwszego miasta.
 * @param id Numer drogi krajowej, której pierwszego miasta szukamy.
 * @return NULL, jeżeli taka droga krajowa nie istnieje, lub wskaźnik na pierwsze miasto drogi krajowej, gdy istnieje.
 */
ListOfCities * getStartCity (Map * map, unsigned id);
/** @brief Funkcja znajdująca kolejny odcinek drogowy drogi krajowej.
 * Funkcja aktualizuje też @p previousCity, tak, aby wskazywał po wykonaniu funkcji na miasto reprezentowane przez origin.
 * @param origin Miasto, z którego kolejnego odcinka szukamy.
 * @param routeId Numer drogi krajowej, której klejnego odcinka szukamy.
 * @param previousCity Wskaźnik na wskaźnik na ostatnio odwiedzone miasto.
 * @return NULL jeżeli to już ostatnie miasto, lub miasto origin nie należy do drogi krajowej numer routeId.
 * Wskaźnik na kolejny odcinek drogowy drogi krajowej, jeżeli udało się taki znaleźć.
 */
City * getNextCityOfRoute (ListOfCities * origin, unsigned routeId, ListOfCities ** previousCity);
/** @brief Przywraca mapę do stanu sprzed zmian na wypadek nieudanego wykonania polecenia.
 * @param original Wskaźnik na oryginalną mapę.
 * @param copy Wskaźnik na kopię mapy.
 */
void repairMap(Map * original, Map * copy);

bool idExists(Map * map, unsigned id);

#endif //DROGI_UTILITIES_H
