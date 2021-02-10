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
* Prends en entrée une liste de sommets
* et crée une liste de face représentant
* un diagramme de Voronoï
*/
template <class S, class T>
class Voronoi{
public:
	vector<Sommet<T>> sommets;//Sommets en entrée
	vector<Triangle<S, T>> triangles;//Triangles en sortie
	vector<Triangle<S, T>> DTL;//Triangles à supprimer de la triangulation
	vector<Triangle<S, T>> NTL;//Triangles à rajouter à la triangulation


	Voronoi(vector<Sommet<T>> sommets):sommets(sommets){}
	~Voronoi(){}

	// Triangule la liste de sommets en entrée
	void triangulate() {
		determiner_triangulation_englobante();
		for (Sommet s : sommets){
			determiner_DTL(s, t);
			determiner_NTL(s);
			supprimer_DTL();
		}
	}

	// Détermine une triangulation qui englobe tous les sommets à trianguler
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

	// Détermine la liste des triangles à supprimer de la triangulation
	void determiner_DTL(Sommet s, Triangle t) {

	}

	// Détermine la liste des triangles à rajouter à la triangulation
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
