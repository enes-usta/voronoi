#pragma once
#include <vector>
#include "Sommet.h"
#include "Vecteur2D.h"
#include "Graphe.h"
#include "Triangulator.h"
#include "EdgeFlipper.h"
#include "Color.h"

/*
* Classe DelaunayTriangulator
* Prends en entre une liste de sommets
* et crée une triangulation de Delaunay
*
* T est la nature de l'information porte par une arête
* S est la nature de l'information porte par une face
*/
template <class S, class T>
class DelaunayTriangulator {
public:
	DelaunayTriangulator() {}
	~DelaunayTriangulator() {}

	vector<Triangle<S, T>*>* triangulate(vector<Sommet<Vecteur2D>*>* sommets, Graphe<T, Vecteur2D>* graphe) {
		Triangulator<S, T> triangulator;
		EdgeFlipper<S, T> edgeFlipper;
		return edgeFlipper.EdgeFlipDelaunay(triangulator.triangulate(sommets, graphe), graphe);
	}

};