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
* Prends en entrée une liste de sommets
* et crée une liste de face représentant
* un une triangulation
*
* S est la nature de l'information portée par une arête
*/
template <class S>
class Triangulation {
public:
	Triangulation(){}
	~Triangulation(){}

	/**
	* Retourne la liste traingulée de la liste de sommets en entrée 
	*/
	vector<Triangle<S, Vecteur2D>> triangulate(vector<Sommet<Vecteur2D>> sommets) {
		init(sommets);
		determiner_triangulation_englobante();
		for (Sommet<Vecteur2D> s : sommets){
			Triangle<S, Vecteur2D> t = determiner_triangle_contenant_sommet();
			determiner_DTL(s, t);
			determiner_NTL(s);
			supprimer_DTL();
		}

		return triangles;
	}


private:
	vector<Sommet<Vecteur2D>> sommets;//Sommets en entrée
	Graphe<S, Vecteur2D> graphe;//Graphe utilisé pour créer des sommets
	vector<Triangle<S, Vecteur2D>> triangles;//Triangles en sortie
	vector<Triangle<S, Vecteur2D>> DTL;//Triangles à supprimer de la triangulation
	vector<Triangle<S, Vecteur2D>> NTL;//Triangles à rajouter à la triangulation

	//Les deux triangles qui forment le rectangle qui englobe tous les points
	Triangle<S, Vecteur2D> triangleEnglobant1;
	Triangle<S, Vecteur2D> triangleEnglobant2;

	/**
	* Initialise les membres
	*/
	void init(vector<Sommet<Vecteur2D>> sommets) {
		this->sommets = sommets;
		triangles = vector<Triangle<S, Vecteur2D>>();
		DTL = vector<Triangle<S, Vecteur2D>>();
		NTL = vector<Triangle<S, Vecteur2D>>();
		graphe = Graphe<S, Vecteur2D>();
	}

	/**
	* Détermine une triangulation qui englobe tous les sommets à trianguler
	*/
	void determiner_triangulation_englobante() {
		/* On cherche les points extrêmes */
		int xMin = sommets.front().v.x;
		int xMax = sommets.front().v.x;
		int yMin = sommets.front().v.y;
		int yMax = sommets.front().v.y;

		for (Sommet<Vecteur2D> s : sommets) {
			(s.v.x < xMin) ? xMin = s.v.x;
			(s.v.y < yMin) ? yMin = s.v.y;
			(s.v.x > xMax) ? xMax = s.v.x;
			(s.v.y > yMax) ? yMax = s.v.y;
		}

		/* On crée les sommets/arêtes d'un rectangle avec ces points */
		Sommet<Vecteur2D>* s0, * s1, * s2, * s3;
		s0 = graphe.creeSommet(Vecteur2D(xMin, yMin));
		s1 = graphe.creeSommet(Vecteur2D(xMax, yMin));
		s2 = graphe.creeSommet(Vecteur2D(xMax, yMax));
		s3 = graphe.creeSommet(Vecteur2D(xMin, yMax));

		Arete a0 = graphe.creeArete("a0", s0, s2);
		Arete a1 = graphe.creeArete("a1", s2, s3);
		Arete a2 = graphe.creeArete("a2", s3, s0);
		Arete a3 = graphe.creeArete("a3", s0, s1);
		Arete a4 = graphe.creeArete("a4", s1, s2);

		/*
		s3		 a1			s2
			-------------
		a2	|	a0	-	|	a4
			|	-		|
		s0	-------------	s1
					a3
		*/

		/* On crée une triangulation de ce rectangle */
		triangleEnglobant1 = Triangle(ArcTU(a0, true), ArcTU(a1, true), ArcTU(a2, true));
		triangleEnglobant2 = Triangle(ArcTU(a0, false), ArcTU(a3, true), ArcTU(a4, true));
		triangles.push_back(triangleEnglobant1);
		triangles.push_back(triangleEnglobant2);
	}

	/**
	* Retourne le triangle contenant le sommet s, retourn null s'il n'en existe pas
	*/
	Triangle<S, T> determiner_triangle_contenant_sommet(Sommet<Vecteur2D> s) {
		for(Triangle<S, Vecteur2D> t : triangles)
			if (t.contientPoint(s))
				return t;

		return null;
	}

	// Détermine la liste des triangles à supprimer de la triangulation
	void determiner_DTL(Sommet<T> s, Triangle<S, T> t) {
		DTL.push_back(t);
		for (int i = 0; i < 3; i++) {
			Triangle<S, T> triangleAdjacent = trouver_triangle_adjacent(t.arcs[i]);
			if (find(DTL.begin(), DTL.end(), triangleAdjacent) != DTL.end()) {
				Cercle<T> cercle = triangleAdjacent.cercle_circonscrit();
				if (cercle.contientPoint(s))
					determiner_DTL(s, triangleAdjacent);
			}
		}
	}

	/**
	* Détermine la liste des triangles à rajouter à la triangulation
	*/
	void determiner_NTL(Sommet<T> s) {
		for(Triangle<S, T> t : DTL) {
			for (int i = 0; i < 2; i++) {
				Triangle<S, T> triangleAdjacent = trouver_triangle_adjacent(t.arcs[i]);
				if (triangleAdjacent == null || find(DTL.begin(), DTL.end(), triangleAdjacent) != DTL.end()) {
					//On crée le nouveau triangle
					vector<ArcTU<S, T>> arcs = vector<ArcTU<S, T>>();
					if (t.arcs[i].estAGauche(s)) {
						arcs[0] = ArcTU<S, T>(t.arcs[i].arete, true);
						arcs[1] = ArcTU<S, T>(Arete<S, T>(t.arcs[i].arete.fin, s), true);
						arcs[2] = ArcTU<S, T>(Arete<S, T>(s, t.arcs[i].arete.debut), true);
					}
					else {
						arcs[0] = ArcTU<S, T>(t.arcs[i].arete, false);
						arcs[1] = ArcTU<S, T>(Arete<S, T>(t.arcs[i].arete.fin, s), false);
						arcs[2] = ArcTU<S, T>(Arete<S, T>(s, t.arcs[i].arete.debut), false);
					}

					this.NTL.push_back(Triangle<S, T>(arcs));
				}
			}
		}
	}

	/**
	* Supprime les triangles en commun dans DTL et Triangles
	*/
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
	* Retourne un triangle adjacent à l'arc a s'il en existe un, sinon retourne null
	*/
	Triangle<S, T> trouver_triangle_adjacent(ArcTU<S, T> a) {
		for(Triangle<S, T> triangle : triangles)
			for(ArcTU<S, T> arc : triangle.arcs)
				if (arc != a && a.debut = arc.debut && a.fin == arc.fin)
					return triangle;

		return null;
	}
};
