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
* Classe Triangulation
* Prends en entr�e une liste de sommets
* et cr�e une liste de face repr�sentant
* un une triangulation
*/
template <class S, class T>
class Triangulation {
public:
	vector<Sommet<T>> sommets;//Sommets en entr�e
	Graphe<char, string> graphe;//Graphe en sortie
	vector<Triangle<S, T>> triangles;//Triangles en sortie
	vector<Triangle<S, T>> DTL;//Triangles � supprimer de la triangulation
	vector<Triangle<S, T>> NTL;//Triangles � rajouter � la triangulation

	//Les deux triangles qui forment le rectangle qui englobe tous les points
	Triangle<S, T> triangleEnglobant1;
	Triangle<S, T> triangleEnglobant2;

	Triangulation(vector<Sommet<T>> sommets):sommets(sommets){}
	~Triangulation(){}

	// Triangule la liste de sommets en entr�e
	void triangulate() {
		determiner_triangulation_englobante();
		for (Sommet s : sommets){
			determiner_DTL(s, triangleEnglobant1);
			determiner_NTL(s);
			supprimer_DTL();
		}
	}

	// D�termine une triangulation qui englobe tous les sommets � trianguler
	void determiner_triangulation_englobante() {
		/* On cherche les points extr�mes */
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

		/* On cr�e les sommets/ar�tes d'un rectangle avec ces points */
		Sommet<string>* s0, * s1, * s2, * s3;
		s0 = graphe.creeSommet("S0");
		s0->coordonnees.x = xMin;
		s0->coordonnees.y = yMin;
		s1 = graphe.creeSommet("S1");
		s1->coordonnees.x = xMax;
		s1->coordonnees.y = yMin;
		s2 = graphe.creeSommet("S2");
		s2->coordonnees.x = xMax;
		s2->coordonnees.y = yMax;
		s3 = graphe.creeSommet("S3");
		s3->coordonnees.x = xMin;
		s3->coordonnees.y = yMax;

		Arete a0 = graphe.creeArete('a0', s0, s2);
		Arete a1 = graphe.creeArete('a1', s2, s3);
		Arete a2 = graphe.creeArete('a2', s3, s0);
		Arete a3 = graphe.creeArete('a3', s2, s1);
		Arete a4 = graphe.creeArete('a4', s1, s0);

		/*
		s3		 a1			s2
			-------------
		a2	|	a0	-	|	a3
			|	-		|
		s0	-------------	s1
				 a4
		*/

		/* On cr�e une triangulation de ce rectangle */
		triangleEnglobant1 = Triangle(Arc(a0, true), Arc(a1, true), Arc(a2, true));
		triangleEnglobant2 = Triangle(Arc(a0, false), Arc(a3, false), Arc(a4, false));
		triangles.push_back(triangleEnglobant1);
		triangles.push_back(triangleEnglobant2);
	}

	// D�termine la liste des triangles � supprimer de la triangulation
	void determiner_DTL(Sommet s, Triangle t) {
		DTL.push_back(t);
		for (int i = 0; i < 3; i++) {
			/*if (find(DTL.begin(), DTL.end(), Tij) != DTL.end()) {
				IF p a l�int�rieur du cercle circonscrit � Tij THEN
					determiner_DTL(s, Tij)
			}*/
		}
	}

	// D�termine la liste des triangles � rajouter � la triangulation
	void determiner_NTL(Sommet s) {
		for (Triangle<S, T> t : DTL) {
			for (int i = 0; i < 2; i++) {
				/* 
				
				if(Tij == VIDE){
					Cr�er un nouveau triangle Tl de sommet p et d�ar�te kij (ar�te commune aux triangles Ti et Tij).
					NTL.push_back(Tl)
				}else if( Tij n'appartient pas � DTL){
					Cr�er un triangle Tl de sommet p et d�ar�te kij
					�tablir l�adjacence entre Tl et Tij
					Rajouter Tl � la liste NT L
				}
				*/
			}
		}
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
		DTL.erase(triangleEnglobant1);
		DTL.erase(triangleEnglobant2);
	}

};
