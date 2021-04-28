/*#include <iostream>
#include <string>
#include "Graphe.h"
#include "Triangle.h"
#include "Cercle.h"
#include "ArcTU.h"
#include "Arete.h"
#include "Vecteur2D.h"
#include <vector>
#include <stdio.h>
#include "Triangulator.h"
#include "GUI.h"
#include <random>
#include "EdgeFlipper.h"
#include "Color.h"


using namespace std;

int main(int argc, char** argv)
{

	Graphe<Color*, Vecteur2D>* graphe = new Graphe<Color*, Vecteur2D>;
	Sommet<Vecteur2D>* a, * b, * c, * d;

	//------------------ on insre des nouveaux sommets isols --------------------

	a = graphe->creeSommet(Vecteur2D(5, 0));
	b = graphe->creeSommet(Vecteur2D(5, 8));
	c = graphe->creeSommet(Vecteur2D(4, 6));
	d = graphe->creeSommet(Vecteur2D(6, 6));

	Arete<Color*, Vecteur2D>* ab, * bc, * ca, * ad, * db;
	Color* color = new Color(127, 127, 127, 0);
	ab = graphe->creeArete(color, a, b);
	bc = graphe->creeArete(color, b, c);
	ca = graphe->creeArete(color, c, a);
	ad = graphe->creeArete(color, a, d);
	db = graphe->creeArete(color, d, b);

	vector <Triangle<Color*, Color*>*>* triangulation = new vector <Triangle<Color*, Color*>*>;
	triangulation->push_back(new Triangle<Color*, Color*> (new ArcTU<Color*>(ab, true), new ArcTU<Color*>(bc, true), new ArcTU<Color*>(ca, true)));
	triangulation->push_back(new Triangle<Color*, Color*> (new ArcTU<Color*>(ad, true), new ArcTU<Color*>(db, true), new ArcTU<Color*>(ab, false)));


	vector<Sommet<Vecteur2D>*>* sommets = new vector<Sommet<Vecteur2D>*>;
	sommets->push_back(a);
	sommets->push_back(b);
	sommets->push_back(c);
	sommets->push_back(d);

	EdgeFlipper<Color*, Color*> ef;

	GUI gui(argc, argv);

	gui.dessiner((vector<Face<Color*, Color*>*>*)ef.EdgeFlipDelaunay(triangulation, graphe), sommets);
	return 0;

}
*/