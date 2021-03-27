#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Sommet.h"
#include "Triangle.h"
#include "Graphe.h"
#include "Cercle.h"

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
		for (Sommet<T> s : sommets){
			Triangle<S, T> t = determiner_triangle_contenant_sommet();
			determiner_DTL(s, t);
			determiner_NTL(s);
			supprimer_DTL();
		}
	}

	Triangle<S, T> determiner_triangle_contenant_sommet(Sommet<T> s) {
		for each (Triangle<S, T> t in triangles)
			if (t.contientPoint(s))
				return t;
		throw new Erreur("Aucun triangle ne contient ce sommet !");
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
	void determiner_DTL(Sommet<T> s, Triangle<S, T> t) {
		DTL.push_back(t);
		for (int i = 0; i < 3; i++) {
			Triangle<S, T> triangleAdjacent = trouver_triangle_adjacent(t.arcs[i]);
			if (find(DTL.begin(), DTL.end(), triangleAdjacent) != DTL.end()) {
				Cercle<T> cercle = determiner_cercle_circonscrit(triangleAdjacent);
				if (cercle.contientPoint(s))
					determiner_DTL(s, triangleAdjacent);
		}
	}

	// D�termine la liste des triangles � rajouter � la triangulation
	void determiner_NTL(Sommet<T> s) {
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

	/**
	* D�termine le cercle circonscrit au triangle t
	*/
	Cercle<T> determiner_cercle_circonscrit(Triangle<S, T> t) {
		Sommet<T> centre;
		/**
		*
		* A COMPLETER
		*
		*/

		//Le rayon est �gal � la distance entre le centre du cercle circonscrit et un des sommets du triangle
		double rayon = sqrt((t.arcs[0].x - s.x) * (t.arcs[0].x - s.x) + (t.arcs[0].y - s.y) * (t.arcs[0].y - s.y));
		return Cercle<T>(centre, rayon);
	}

	/**
	* Retourne un triangle adjacent � l'arc a s'il en existe un, sinon retourne null
	*/
	Triangle<S, T> trouver_triangle_adjacent(Arc<S, T> a) {
		for each (Triangle<S, T> triangle in triangles)
			for each (Arc<S, T> arc  in triangle.arcs)
				if (arc != a && a.debut = arc.debut && a.fin == arc.fin)
					return triangle;

		return null;
	}
	
};
