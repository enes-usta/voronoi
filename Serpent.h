#pragma once
#include <vector>
#include "Sommet.h"
#include "Vecteur2D.h"
#include "Graphe.h"
#include "Triangulator.h"
#include "Color.h"
#include <FileLoader.h>


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
		charger_contours();
		//triangulate_germes();
	}

private:

	void charger_contours() {
		FileLoader f("C:\\dev\\projects\\voronoi\\ressources\\Nuage_contour.txt");
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

		faces->push_back(new Face<Color*, Color*>(arcs_contours, face_color));
	}
/*
	void triangulate_germes() {
		Triangulator<char, char> triangulator;
		cellules = (vector<Face<Color*, Color*>*>*) triangulator.triangulate(germes, graphe);
		colorier();
	}

	void colorier() {
		for (Face<Color*, Color*>* f : (*cellules)) {
			f->v = face_color;
			for (ArcTU<Color*>* arc : f->arcs)
				arc->arete->v = edge_color;
		}
	}*/
};