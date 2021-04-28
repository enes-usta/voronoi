#pragma once
#include <vector>
#include "Sommet.h"
#include "Vecteur2D.h"
#include "Graphe.h"
#include "Triangulator.h"
#include "Color.h"

class Tortue {
public:
	vector<Sommet<Vecteur2D>*> *germes;
	vector<Face<Color*, Color*>*> *cellules;
	Graphe<char, Vecteur2D> * graphe;	
	Color* color;

	Tortue() {
		color = new Color(224, 192, 103, 0);
		germes = new vector<Sommet<Vecteur2D>*>();
		graphe = new Graphe<char, Vecteur2D>();
		init_germes();
		triangulateGermes();
	}

private:

	void init_germes() {
		germes->push_back(graphe->creeSommet(Vecteur2D(0, 0)));
		germes->push_back(graphe->creeSommet(Vecteur2D(0, 2)));
		germes->push_back(graphe->creeSommet(Vecteur2D(0, -2)));
		germes->push_back(graphe->creeSommet(Vecteur2D(0, 4)));
		germes->push_back(graphe->creeSommet(Vecteur2D(0, -4)));

		germes->push_back(graphe->creeSommet(Vecteur2D(2, -3)));
		germes->push_back(graphe->creeSommet(Vecteur2D(-2, -3)));
		germes->push_back(graphe->creeSommet(Vecteur2D(3, -1)));
		germes->push_back(graphe->creeSommet(Vecteur2D(-3, -1)));
		germes->push_back(graphe->creeSommet(Vecteur2D(3, 1)));
		germes->push_back(graphe->creeSommet(Vecteur2D(-3, 1)));
		germes->push_back(graphe->creeSommet(Vecteur2D(2, 3)));
		germes->push_back(graphe->creeSommet(Vecteur2D(-2, 3)));
	}

	void triangulateGermes() {
		Triangulator<char, char> triangulator;
		cellules = (vector<Face<Color*, Color*>*>*) triangulator.triangulate(germes, graphe);
		colorierCellules();
	}

	void colorierCellules() {
		for (Face<Color*, Color*>* f : (*cellules)) {
			for (ArcTU<Color*> *arc : f->arcs) {
				arc->arete->v = color;
			}
		}
	}
};