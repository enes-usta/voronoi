#pragma once
#include <vector>
#include "Sommet.h"
#include "Vecteur2D.h"
#include "Graphe.h"
#include "Triangulator.h"
#include "Color.h"
#include <FileLoader.h>

#define MAX 10000

class Serpent {
public:
	vector<Sommet<Vecteur2D>*>* germes;
	vector<Face<Color*, Color*>*>* faces;
	Face<Color*, Color*>* contour;
	Graphe<Color*, Vecteur2D>* graphe;
	Color* edge_color, * face_color;

	Serpent() {
		edge_color = new Color(255, 255, 255, 0);
		face_color = new Color(255, 255, 0, 0);
		germes = new vector<Sommet<Vecteur2D>*>();
		graphe = new Graphe<Color*, Vecteur2D>();
		faces = new vector<Face<Color*, Color*>*>;
		charger_geometries();
	}

private:
	void charger_geometries() {
		charger_ecailles();
		charger_contour();
		faces->push_back(contour);
		clipping();
	}

	void charger_contour() {
		FileLoader f(".\\ressources\\Nuage_contour.txt");
		vector<Sommet<Vecteur2D>*> contours;

		for (Vecteur2D v : f.listeSommets) {
			contours.push_back(graphe->creeSommet(v));
		}

		vector<ArcTU<Color*>*> arcs_contours;

		int i = 0;
		for (Sommet<Vecteur2D>* s : contours){
			int j = (i + 1) % contours.size();
			Arete<Color*, Vecteur2D>* a = graphe->creeArete(edge_color, contours[i], contours[j]);
			arcs_contours.push_back(new ArcTU<Color*>(a, true));
			i++;
		}

		contour = new Face<Color*, Color*>(arcs_contours, NULL);
	}

	void charger_ecailles() {
		FileLoader f(".\\ressources\\Nuage_noyaux_ecailles.txt");

		for (Vecteur2D v : f.listeSommets) {
			germes->push_back(graphe->creeSommet(v));
		}

		Triangulator<Color*, Color*> triangulator;
		faces = (vector<Face<Color*, Color*>*>*) triangulator.triangulate(germes, graphe);
	}

	void clipping() {
		for (auto it = faces->begin(); it != faces->end(); ) {
			bool deleted = false;
			Triangle<Color*, Color*>* t = (Triangle<Color*, Color*>*) * it;
			int nbIntersection = 0;
			for (ArcTU<Color*>* arc : (t->arcs)) {
				Vecteur2D centre = t->cercle_circonscrit().centre;
				for (ArcTU<Color*>* a : contour->arcs) {
					if (Geometrie::intersection(centre, Vecteur2D(MAX, MAX), a->debut()->v, a->fin()->v)) {
						nbIntersection++;
					}
				}
			}
			if (nbIntersection % 2 == 0) {
				delete* it;
				it = faces->erase(it);
				deleted = true;
				goto next;
			}
		next:
			if (!deleted)
				++it;
		}
	}
};