#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Sommet.h"
#include "Triangle.h"
#include "Graphe.h"

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
	Graphe<char, string> graphe;//Graphe en sortie
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
		int xMin = sommets.front().coordonnees.x;
		int xMax = sommets.front().coordonnees.x;
		int yMin = sommets.front().coordonnees.y;
		int yMax = sommets.front().coordonnees.y;
		for (Sommet s : sommets) {
			(s.coordonnees.x < xMin) ? xMin = s.coordonnees.x;
			(s.coordonnees.y < yMin) ? yMin = s.coordonnees.y;
			(s.coordonnees.x > xMax) ? xMax = s.coordonnees.x;
			(s.coordonnees.y > yMax) ? yMax = s.coordonnees.y;
		}

		Sommet<string>* s0, * s1, * s2, * s3;
		s0 = graphe.creeSommet("S0");
		s1 = graphe.creeSommet("S1");
		s2 = graphe.creeSommet("S2");
		s3 = graphe.creeSommet("S3");

		Arete a0 = graphe.creeArete('a0', s0, s2);
		Arete a1 = graphe.creeArete('a1', s2, s3);
		Arete a2 = graphe.creeArete('a2', s3, s0);
		Arete a3 = graphe.creeArete('a3', s2, s1);
		Arete a4 = graphe.creeArete('a4', s1, s0);

		Triangle t1();
		Triangel t2();
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
