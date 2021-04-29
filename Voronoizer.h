#pragma once
#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Sommet.h"
#include "Triangle.h"
#include "Graphe.h"
#include "Cercle.h"
#include "Face.h"
#include "Triangulator.h"


using namespace std;

/*
* Classe Voronoizer
* Prends en entre une liste de sommets
* et crée un diagramme de Voronoï
*
* T est la nature de l'information porte par une arête
* S est la nature de l'information porte par une face
*/
template <class S, class T>
class Voronoizer {
public:
	Voronoizer() {
		this->_germes = new vector<Sommet<Vecteur2D>*>;
	}
	~Voronoizer() {}

	/**
	* Retourne la liste traingule de la liste de sommets en entre
	*/
	vector<Face<S, T>*>* voronoize(vector<Sommet<Vecteur2D>*>* sommets, Graphe<T, Vecteur2D>* graphe) {
		this->graphe = graphe;
		Triangulator<S, T> triangulator;
		vector<Triangle<S, T>*>* delaunayTriangulation = triangulator.triangulate(sommets, graphe);
		calculer_germes(*delaunayTriangulation);
		return (vector<Face<S, T>*>*) delaunayTriangulation; // à changer
	}

	vector<Sommet<Vecteur2D>*>* germes() {
		return this->_germes;
	}

private:
	vector<Sommet<Vecteur2D>*>* _germes;
	Graphe<T, Vecteur2D>* graphe;

	void calculer_germes(vector<Triangle<S, T>*> & triangulation) {
		for (Triangle<S, T>* triangle : triangulation) {
			Sommet<Vecteur2D>* s = graphe->creeSommet(triangle->cercle_circonscrit().centre);
			_germes->push_back(s);
		}
	}

};
