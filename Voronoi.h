#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Sommet.h"
#include "Triangle.h"


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
	vector<Triangle<S, T>> triangles;//Triangles en sortie
	vector<Triangle<S, T>> DTL;//Triangles � supprimer de la triangulation
	vector<Triangle<S, T>> NTL;//Triangles � rajouter � la triangulation


	Voronoi(vector<Sommet<T>> sommets):sommets(sommets){}
	~Voronoi(){}

	// Triangule la liste de sommets en entr�e
	void triangulate() {
		determiner_triangulation_englobante();
		for (Sommet s : sommets){
			determiner_DTL(s, t);
			determiner_NTL(s);
			supprimer_DTL();
		}
	}

	// D�termine une triangulation qui englobe tous les sommets � trianguler
	void determiner_triangulation_englobante() {
		int xMin = sommets.front().getX();
		int xMax = sommets.front().getX();
		int yMin = sommets.front().getY();
		int yMax = sommets.front().getY();
		for (Sommet s : sommets) {
			(s.getX() < xMin) ? xMin = s.getX();
			(s.getY() < yMin) ? yMin = s.getY();
			(s.getX() > xMax) ? xMax = s.getX();
			(s.getY() > yMax) ? yMax = s.getY();
		}
	}

	// D�termine la liste des triangles � supprimer de la triangulation
	void determiner_DTL(Sommet s, Triangle t) {

	}

	// D�termine la liste des triangles � rajouter � la triangulation
	void determiner_NTL(Sommet s) {

	}
	
	// Supprime les triangles en commun dans DTL et Triangles
	void supprimer_DTL() {
		for (i = DTL.begin(); i != DTL.end(); i++) {
			for (j = Triangles.begin(); j != Triangles.end(); j++) {
				if (*i == *j) {
					DTL.erase(i);
					Triangles.erase(j);
					break;
				}
			}
		}
	}

};
