/** @file
 * Plik ze strukturami używanymi w programie.
 * @author Kacper Kuta <k.kuta2@student.uw.edu.pl>
 * @date 8.04.2019
 */

#ifndef DROGI_STRUCTURES_H
#define DROGI_STRUCTURES_H
//
// Created by baka475 on 28.04.19.
//


/** @brief Struktura głównej list miast.
 * Struktura zawiera identyfikator miasta cityID, wskaźnik na następne miasto next, oraz wskaźnik na listę sąsiadów neighbour.
 */
typedef struct ListOfCities {
    /** @brief  Identyfikator miasta*/
    int cityID;
    /** @brief  Wksaźnik na listę wychodzących odcinków drogowych*/
    struct City * neighbour;
    /** @brief  Wskaźnik na następny element listy @p ListOfCities*/
    struct ListOfCities * next;
} ListOfCities;
/** @brief Lista numerów dróg krajowych.
 * firstCity może wskazywać na pierwsze miasto drogi krajowej.
 */
typedef struct listOfRouteIDs {
    /** @brief  Numer drogi krajowej*/
    unsigned routeID;
    /** @brief  Opcjonalne pierwsze miasto drogi krajowej*/
    ListOfCities * firstCity;
    /** @brief  Wskaźnik na kolejny element listy @p listOfRouteIDs*/
    struct listOfRouteIDs * next;
} listOfRouteIDs;
/** @brief Lista odcinków drogowych.
 * Zawiera pola:
 * cityID - identyfikator miasta
 * distance - długość odcinka drogowego
 * buildYear - rok budowy lub ostatniej naprawy
 * next - wskaźnik na następny odcinek drogowy
 * oppositeWay - wskaźnik na odcinek drogowy prowadzący w przeciwną stronę
 * routes - wskaźnik na strukturę @p listOfRouteIDs
 * mainListRep - reprezentacja miasta docelowego odcinka w liście głownej miast @p ListOfCities
 */
typedef struct City {
    /** @brief Identyfikator miasta */
    int cityID;
    /** @brief Długość odcinka drogowego */
    unsigned distance;
    /** @brief rok budowy lub ostatniego remontu */
    int buildYear;
    /** @brief  wskaźnik na następny element*/
    struct City * next;
    /** @brief  wskaźnik na odcinek w przeciwnym kierunku*/
    struct City * oppositeWay;
    /** @brief  wskażnik na listę numerów dróg krajowych @p listOfRouteIDs*/
    listOfRouteIDs * routes;
    /** @brief  Wskaźnik na główną reprezentację miasta docelowego*/
    ListOfCities * mainListRep;
} City;
/** @brief Lista odcinków drogowych tworzących ścieżkę
 */
typedef struct Path {
    /** @brief Wskaźnik na odcinek drogowy. */
    City * city;
    /** @brief  Wskaźnik na następny element ścieżki.*/
    struct Path * next;
} Path;
/** @brief Lista ścieżek - list odcinków drogowych @p Path
 */
typedef struct ListOfPaths {
    /** @brief  Wskaźnik na ścieżkę @p Path */
    Path * path;
    /** @brief  Wskaźnik na następny element listy*/
    struct ListOfPaths * next;
    /** @brief  Wskaźnik na ostatni element listy*/
    struct ListOfPaths * last;
} ListOfPaths;
/** @brief Struktura algorytmu dijkstry, trzymająca informację o minimalnym dystansie i informacji o odwiedzeniu.
 */
typedef struct dijkstra {
    /** @brief  Informacja, czy miasto było już odwiedzone*/
    int visited;
    /** @brief  Minimalna znaleziona odległość od miasta*/
    unsigned minDistance;
} dijkstra;
/** @brief Struktura kolejki priorytetowej
 */
typedef struct priorityQueue {
    /** @brief  Wskaźnik na miasto - element w kolejce*/
    ListOfCities * city;
    /** @brief  Waga elementu*/
    int weight;
    /** @brief  Następny element w kolejce*/
    struct priorityQueue * next;
} priorityQueue;
/** @brief Lista liczb całkowitych.
 */
typedef struct listOfInts {
    /** @brief Liczba */
    int element;
    /** @brief  Wskaźnik na następny element listy*/
    struct listOfInts * next;
} listOfInts;
/** @brief Struktura mapy.
 * Zawiera pola:
 * firstCity - wskaźnik na listę miast @p ListOfCities
 * citiesID - tablica idenyfikatorów miast
 * citiesIDlength - długość tablicy identyfikatorów
 * IDs - wskaźnik na listę @p listOfRouteIDs z numerami dróg krajowych
 */
struct Map {
    /** @brief  Wskaźnik na listę miast @p ListOfCities*/
    ListOfCities * firstCity;
    /** @brief  Tablica identyfikatorów miast*/
    const char ** citiesID;
    /** @brief  Długość tablicy identyfikatorów*/
    int citiesIDLength;
    /** @brief  Lista dróg krajowych wraz z pierwszymi miastami - @p listOfRouteIDs*/
    listOfRouteIDs * IDs;
};

#endif //DROGI_STRUCTURES_H
