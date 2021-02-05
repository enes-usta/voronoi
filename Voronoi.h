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
* Prends en entrée une liste de sommets
* et crée une liste de face représentant
* un diagramme de Voronoï
*/
template <class S, class T>
class Voronoi{
public:
	vector<Sommet<T>> sommets;//Sommets en entrée
	vector<Face<S, T>> faces;//Face en sortie

	Voronoi(vector<Sommet<T>> sommets):sommets(sommets){}
	~Voronoi(){}
	void triangulate();
};
