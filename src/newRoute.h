/** @file
 * Interfejs klasy funkcji używanych do tworzenia i wydłużania drogi krajowej oraz usuwania odcinków drogowych.
 * @author Kacper Kuta <k.kuta2@student.uw.edu.pl>
 * @date 8.04.2019
 */

#ifndef DROGI_NEWROUTE_H
#define DROGI_NEWROUTE_H
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
/** @brief
 * Wystapił problem z alokacją pamięci.
 */
#define MEMORY_PROBLEM -5

/** @brief Funkcja dodająca na początek każdej ścieżki odcinek drogogwy.
 * @param toAdd Wskaźnik na odcinek drogowy do dodania.
 * @param paths Wskaźnik na listę ścieżek, do których chcemy dodać odcinek
 * @return @p true, jeżeli paths był NULLem, bądź udało się dodać do wszystkich ścieżek odcinek drogowy.
 * @p false, jeżeli nie udało się zaalokować pamięci.
 */
bool addFirstToEachPath (City * toAdd, ListOfPaths * paths);
/** @brief Funkcja tworząca nową listę ścieżek z jedną ścieżką i jednym odcinkiem drogowym.
 * @param previous Odcinek drogowy, który będzie jedynym w jedynej ścieżce, w nowej liście ścieżek.
 * @return @p true jeżeli udało się stworzyć listę ścieżek, @p false jeżeli nie udało się zaalokować pamięci.
 */
ListOfPaths * endOfPath (City * previous);
/** @brief Tworzy pustą listę ścieżek
 * @return NULL jeżeli się nie uda zaalokować pamięci, wskaźnik na pustą listę ścieżek wpp.
 */
ListOfPaths * createEmpty (void);
/** @brief Znajduje wszytkie ścieżki o danej długości.
 * Znajduje wszystkie ścieżki z @p city do @p finalCity o długości length.
 * @param map Mapa, w której ścieżek szukamy.
 * @param length Długość pozostała do celu.
 * @param city Wskaźnik na miasto startowe.
 * @param finalCity Wskaźnik na miasto docelowe.
 * @param previous Wskaźnik na odcinek drogowy, którym przyszliśmy do miasta startowego.
 * NULL, jeżeli jesteśmy w pierwszym mieście.
 * @return Lista ścieżek, jeżeli uda się jakąkolwiek znaleźć,
 * pusta lista ścieżek, generowana przez @ref createEmpty(), jeżeli nie udało się znależć żadnej ścieżki,
 * NULL w przypadku problemu z pamięcią.
 */
ListOfPaths * findAllEqualPaths (Map * map, int length, ListOfCities * city, ListOfCities * finalCity, City * previous);
/** @brief Zlicza miasta w mapie.
 * @param map Wskaźnik na mapę, której miasta zliczamy.
 * @return Liczba miast w mapie.
 */
int amountOfCities(Map * map);
/** @brief Znajduje najlepszą ścieżę w liście ścieżek.
 * @param paths Wskaźnik na listę ścieżek.
 * @return NULL jeżeli takiej ścieżki nie da się jednoznacznie określić, lub lista jest pusta.
 * Wskaźnik na najlepszą ścieżkę, jeżeli się uda ją znaleźć.
 */
Path * chooseBest(ListOfPaths * paths);
/** @brief Znajduje najlepszą ścieżkę między podanymi miastami.
 * @param map Wskaźnik na mapę, w której szukamy ścieżki.
 * @param origin Wskaźnik na misato startowe.
 * @param destination Wskaźnik na miasto docelowe.
 * @return Wskaźnik na najlepszą ścieżkę, jeżeli uda się znaleźć, NULL wpp.
 */
Path * bestPath (Map * map, ListOfCities * origin, ListOfCities * destination);
/** @brief Łączy dwie listy ścieżek w jedną.
 * @param first Wskażnik na pierwszą listę ścieżek.
 * @param second Wskaźnik na drugą listę ścieżek.
 * @return Wskaźnik na połączone listy ścieżek.
 * NULL jeżeli którykolwiek wskaźnik na listę ścieżek był NULLem.
 */
ListOfPaths * connectListOfPaths (ListOfPaths * first, ListOfPaths * second);
/** @brief Inicjalizuje tablicę odwiedzonych potrzebną w funkcji @ref shortestDistanceForExtend.
 * Alokuje pamięć we wszystkich komórkach tablicy na struktury dijkstra.
 * Dla wszytskich pól poza polem o indeksie identyfikatora miasta origin ustawia pole visited na @p false oraz minDistance na INT_MAX.
 * Dla pola o indeksie identifikatora miasta @p origin ustawia visited jako @p true oraz minDistance jako 0;
 * @param tableLength Długość tablicy - liczba miast w mapie.
 * @param visitedTable Wskaźnik na adres nowej tablicy odwiedzonych.
 * @param origin Wskaźnik na miasto startowe.
 * @return @p true jeżeli wszystko się powiedzie, MEMORY_PROBLEM, czyli -5 gdy wystąpi problem z alokacją pamięci.
 */
int initializeVisitedTable (int tableLength, dijkstra ** visitedTable, ListOfCities * origin);
/** @brief Sprawdza, czy istnieje droga krajowa o danym numerze.
 * @param map Wskaźnik na mapę, w której droga miałaby istnieć.
 * @param id Sprawdzany numer drogi krajowej.
 * @return @p true jeżeli istnieje taka droga krajowa, @p false wpp.
 */
bool idExists(Map * map, unsigned id);
/** @brief Dodaje numer drogi krajowej do struktury mapy.
 * @param id Numer drogi krajowej.
 * @param map Wskaźnik na mapę.
 * @param first Wskaźnik na odcinek drogowy prowadzący do pierwszego miasta drogi krajowej.
 * @return @p false w przypadku problemów z pamięcią, @p true gdy się uda.
 */
bool addIDToMap (unsigned id, Map * map, City * first);
/** @brief Do każdego odcinka nowej drogi krajowej dodaje jej numer. Wywołuje funkcję @ref addIDToMap.
 * @param routeID Numer drogi krajowej.
 * @param map Wskaźnik na mapę.
 * @param path Wskaźnik na ścieżkę z odcinków dogowych nowej drogi krajowej.
 * @return @p true, jeżeli operacja się powiedzie, @p false w przypadku problemu z pamięcią.
 */
bool createRoute(unsigned routeID, Map * map, Path * path);
#endif //DROGI_NEWROUTE_H
