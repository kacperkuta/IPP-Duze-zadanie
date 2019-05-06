/** @file
 * Interfejs klasy fuknkcji używanych do tworzenia nowego odcinka drogowego.
 * @author Kacper Kuta <k.kuta2@student.uw.edu.pl>
 * @date 8.04.2019
 */


#ifndef DROGI_ADDREPAIR_H
#define DROGI_ADDREPAIR_H
#include "map.h"
#include "structures.h"

/** @brief Tworzy identifikator dla nowego miasta.
 * @param city Wskaźnik na nazwę miasta.
 * @param citiesID Wskaźnik na adres tablicy z nazwami miast i identyfikatorami.
 * @param citiesIDLength  Wskaźnik na dotychczasową długość tablicy identyfikatorów i miast.
 * @return Zwraca identyfikator w przypadku powodzenia. Jeżeli nie uda się zaalokować pamięci zwraca -1.
 */
int createIDForCity(const char * city, const char *** citiesID, int * citiesIDLength);
/** @brief Dodaje miasto do listy miast w strukturze @p map.
 * Wywołuje funkcję @ref createIDForCity w celu utworzenia identyfikatora. Następnie dodaje miasto do struktury mapy.
 * @param map Wskaźnik na mapę, do której miasto dodajemy.
 * @param city Wskaźnik na napis z nazwą miasta.
 * @return @p true jeżeli operacja siępowiedzie, @p false w przypadku problemów z pamięcią.
 */
bool addCityToMap (Map * map, const char * city);
/** @brief Usuwa pierwsze miasto ze struktury mapy.
 * Nie zwalnia pamięci zajmowanej przez listę odcinków drogowych wychodzących z tego miasta.
 * @param map Wskaźnik na mapę, z której miasto usuwamy.
 */
void removeFirstCity(Map * map);
/** @brief Dodaje odcinki drogowe niezbędne do połączenia dwóch miast.
 * @param map Wskaźnik na mapę, do której odcinki dodajemy.
 * @param city1 Wskaźnik na odcinek drogowy z @p city2 do @p city1.
 * @param city2 Wskaźnik na odcinek drogowy w przeciwną stronę.
 * @param city1ID Identyfikator pierwszego miasta.
 * @param city2ID Identyfikator drugiego miasta
 * @return @p true, jeżeli operacja się powiedzie, @p false w przypadku problemów z pamięcią.
 */
bool createRoad (Map * map, City * city1, City * city2, int city1ID, int city2ID);
/** @brief Tworzy nową reprezentację odcinka drogowego.
 * @param cityID Identyfikator docelowego miasta
 * @param distance Dystans do docelowego miasta/
 * @param buildYear Rok budowy odcinka.
 * @param mainListRep Wskaźnik na reprezentację miasta docelowego w liście miast w strukturze mapy.
 * @return NULL w przypadku niezaalokowania pamięci. Wskaźnik na odcinek drogowy przy powodzeniu.
 */
City * createNeighbour (int cityID, unsigned distance, int buildYear, ListOfCities * mainListRep);
#endif //DROGI_ADDREPAIR_H
