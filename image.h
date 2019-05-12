#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <string>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <array>
#include "sudoku.h"

// Funkcija ucitava sliku sa date putanje.
std::vector<std::vector<unsigned char>> readImage(std::string pathname);
// Funkcija prepoznaje linije sudokua i locira mjesta gdje se nalaze brojevi.
// Ucitava brojeve u polja sudokua.
void lineRecognition(Sudoku& sudoku, std::vector<std::vector<unsigned char>> buffer);
// pomocne funkcije:
// Da li postoji horizontalna linija u tom polju?
bool horizontalLine(int ul_r, int ul_c, int h, int w, std::vector<std::vector<unsigned char>> image);
// Vertikalna?
bool verticalLine(int ul_r, int ul_c, int h, int w, std::vector<std::vector<unsigned char>> image);
// Da li polje ima liniju samo s leve strane. Sa desne da nema crnih piksela...
bool emptyRight(int ul_r, int ul_c, int h, int w, std::vector<std::vector<unsigned char>> image);
// broj crnih piksela u polju
int pixels(int ul_r, int ul_c, int h, int w, std::vector<std::vector<unsigned char>> image);
// Ojlerova funkcija iz knjige
int eulerTest(int ul_r, int ul_c, int h, int w, std::vector<std::vector<unsigned char>> image);
// funkcija smanjuje kockicu na najmanju mogucu i prepoznaje broj (ako moze)
int numberRecognition(int ul_r, int ul_c, int h, int w, std::vector<std::vector<unsigned char>> image);
#endif
