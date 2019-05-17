/** @file
 * Interfejs klasy funkcji używanych do wczytywania poleceń z pliku wejściowego.
 * @author Kacper Kuta <k.kuta2@student.uw.edu.pl>
 * @date 8.05.2019
 */

#ifndef DROGI_READLIBRARY_H
#define DROGI_READLIBRARY_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "map.h"

/** @brief Stała oznaczająca polecenie dodawania nowego odcinka, równa -10 */
#define ADD -10
/** @brief Stała oznaczająca polecenie naprawienia odcinka drogowego, równa -11 */
#define REPAIR -11
/** @brief Stała oznaczająca polecenie wywołania funkcji getRouteDecription równa -12 */
#define DESCRIPTION -12
/** @brief Stała oznaczająca nieporawidłowe polecenie równa -13 */
#define INCORRECT_COMMAND -13

/** @brief Funkcja zarządzająca pamięcią.
 * W razie potrzeby rozszerza pamięć wskaźnika na napis.
 * @param description Wskaźnik na napis.
 * @param length Wskaźnik na aktualną długość napisu.
 * @param expectedLength Spodziewany rozmiar tablicy.
 * @return @p true, jeżeli pamięć uda się zaalokować lub nie była wymagana akcja.
 * @p false jeżeli wystąpi błąd z alokacją pamięci.
 */
bool extendMemory (char ** description, int * length, int expectedLength);
/** @brief Wczytuje napis aż do natrafienia na średnik lub znak nowej linii '\n' lub EOF.
 * Pozostawia ostatni znak kończący wczytwanie na wejściu.
 * @return Wskaźnik na ten napis. NULL w przypadku problemu z alokacją pamięci lub jeśli nie było znaków do wczytania.
 */
const char * readToSemicolon ();
/** @brief Sprawdza, czy napis jest liczbą
 * @param inscription wskaźnik na napis
 * @return @p true jeżeli napis jest liczbą
 * @p false wpp
 */
bool isNumber (const char * inscription);
/**@brief Konwertuje string będący liczbą całkowitą na inta.
 * @param string Wskaźnik na konwertowany napis.
 * @return Przekonwertowany napis na inta.
 */
int convertStringToInteger (const char * string);
/** @brief Informuje o otrzymanym poleceniu.
 * Sprawdza, jakie polecenie stoi przed pierwszym średnikiem w linii.
 * @param inscription Wskaźnik na napis odpowiadający poleceniu.
 * @return @p ADD jeżeli polecenie to addRoute.
 * @p REPAIR jeżeli polecenie to repairRoute
 * @p DESCRIPTION jeżeli polecenie to getRouteDescription.
 * liczbę dodatnią, jeżeli polecenie jest liczbą całkowitą dodatnią
 * @p INCORRECT_INSTRUCTION wpp
 */
int commandType (const char * inscription);
/** @brief Sprawdza, czy następny znak to koniec linii.
 * Jeżeli nie, to wczytuje aż do napotkania EOF lub końca linii.
 * @return @p true, jeżeli kolejny znak to był znak końca linii. @p false wpp.
 */
bool checkEOL ();
/** @brief Sprawdza czy kolejny znak to średnik.
 * Jeżeli nie, wywołuje @ref checkEOL.
 * @return @p true jeżeli następny był średnik, @p false wpp
 */
bool checkSemicolon ();
/** @brief Zwalnia pamięć wskaźnika, gdy ten nie jest NULLem.
 * @param pointer Wskaźnik na pamięć do zwolnienia.
 */
void freeIfNotNULL (const char * pointer);

#endif //DROGI_READLIBRARY_H
