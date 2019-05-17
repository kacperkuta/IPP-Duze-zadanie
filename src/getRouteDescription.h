/** @file
 * Interfejs klasy funkcji używanych do tworzenia opisów dróg krajowych.
 * @author Kacper Kuta <k.kuta2@student.uw.edu.pl>
 * @date 8.04.2019
 */
#ifndef DROGI_GETROUTEDESCRIPTION_H
#define DROGI_GETROUTEDESCRIPTION_H

/** @brief Początkowy rozmiar tablicy, wystarczający dla większości sensownych nazw */
#define START_SIZE 32
#include "structures.h"
#include "map.h"

/** @brief Dodaje numer drogi krajowej do napisu.
 * Aktualizuje descLength na aktualną długość.
 * @param string Wskaźnik na napis.
 * @param id Numer drogi krajowej
 * @param descLength Wskaźnik na długość napisu.
 */
void addIntToString(char * string, unsigned id, int * descLength);
/** @brief Dodaje odcinek drogowy do opisu.
 * Aktualizuje descLength na aktualną długość.
 * @param map Wskaźnik na mapę
 * @param city Wskaźnik na odcinek drogowy
 * @param description Wskaźnik na tablicę z napisem
 * @param size Wskaźnik na aktualny rozmiar tablicy description
 * @param descLength Wskaźnik na aktualną długość napisu
 * @return @p true gdy się uda dodać, @p false gdy wystąpi problem z pamięcią.
 */
bool addCityToDescription(Map * map, City * city, char ** description, int * size, int * descLength);
/** @brief Dodaje nazwę pierwszego miasta do napisu.
 * @param map Wskaźnik na mapę
 * @param city Wskaźnik na pierwsze miasto
 * @param description Wskaźnik na tablicę z napisem
 * @param size Wskaźnik na aktualny rozmiar tablicy
 * @param descLength Wskaźnik na aktualny rozmiar napisu
 * @return @p true jeżeli się uda dodać miast, @p false gdy wystąpi rpoblem z pamięcią
 */
bool addFirstCity (Map * map, ListOfCities * city, char ** description, int * size, int * descLength);
/** @brief Generuje pusty napis.
 * Zwalnia pamięć starego napisu.
 * @param old Wskaźnik na stary napis.
 * @return Wskaźnik na pusty napis. Jeżeli alokacja pamięci się nie powiedzie zwraca NULL;
 */
const char * generateEmpty (char * old);
#endif //DROGI_GETROUTEDESCRIPTION_H
