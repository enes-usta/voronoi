#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Sommet.h"
#include "Face.h"

using namespace std;

/*
* Classe Voronoi
* Prends en entr�e une liste de sommets
* et cr�e une liste de face repr�sentant
* un diagramme de Vorono�
*/
template <class S, class T>
class Voronoi{
public:
	vector<Sommet<T>> sommets;//Sommets en entr�e
	vector<Face<S, T>> faces;//Face en sortie

	Voronoi(vector<Sommet<T>> sommets):sommets(sommets){}
	~Voronoi(){}
	void triangulate();
};
