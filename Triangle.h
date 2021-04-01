#pragma once
#include <vector>
#include "Sommet.h"
#include "Arc.h"
#include "Face.h"
#include "Matrice.h"
#include "Vecteur2D.h"
#include "Cercle.h"

/**
* S est la nature de l'information portée par une arête
*/
template <class S>
class Triangle : public Face<S> {
public:
    Triangle(vector<Arc<S>> arcs) : Face<S>(arcs) {}

    Triangle(Arc<S> a1, Arc<S> a2, Arc<S> a3) {
        vector<Arc<S>> a = vector<Arc<S>>();
        a.push_back(a1);
        a.push_back(a2);
        a.push_back(a3);
        super(a);
    }

    /**
    * Retourne le cercle circonscrit de ce triangle
    */
    Cercle cercle_circonscrit() {

        Vecteur2D a = this->arcs[0].arete->debut->v;
        Vecteur2D b = this->arcs[1].arete->debut->v;
        Vecteur2D c = this->arcs[2].arete->debut->v;

        Vecteur2D centre = Vecteur2D();
        // Les formules : https://en.wikipedia.org/wiki/Circumscribed_circle#Circumcenter_coordinates
        double d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
        centre.x = ((a.x * a.x + a.y * a.y) * (b.y - c.y) + (b.x * b.x + b.y * b.y) * (c.y - a.y) + (c.x * c.x + c.y * c.y) * (a.y - b.y)) / d;
        centre.y = ((a.x * a.x + a.y * a.y) * (c.x - b.x) + (b.x * b.x + b.y * b.y) * (a.x - c.x) + (c.x * c.x + c.y * c.y) * (b.x - a.x)) / d;

        //Un sommet du triangle
        Vecteur2D s = this->arcs[0].arete->debut->v;

        //Le rayon est égal à la distance entre le centre du cercle circonscrit et un des sommets du triangle
        double rayon = sqrt((s.x - centre.x) * (s.x - centre.x) + (s.y - centre.y) * (s.y - centre.y));
        return Cercle(centre, rayon);
    }
};
