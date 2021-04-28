#pragma once
#include <vector>
#include "Sommet.h"
#include "ArcTU.h"
#include "Face.h"
#include "Vecteur2D.h"
#include "Cercle.h"

/**
* T est la nature de l'information portée par une arête
* S est la nature de l'information portée par cette face
*/
template <class S, class T>
class Triangle : public Face<S, T> {
public:

    Triangle(){}

    Triangle(vector<ArcTU<T>*> arcs, const S& v) : Face<S, T>(arcs, v) {}

    Triangle(ArcTU<T>* a1, ArcTU<T>* a2, ArcTU<T>* a3) : Face<S, T>() {
        this->arcs.push_back(a1);
        this->arcs.push_back(a2);
        this->arcs.push_back(a3);
    }

    /**
    * Retourne le cercle circonscrit de ce triangle
    */
    Cercle cercle_circonscrit() {

        Vecteur2D a = this->arcs[0]->debut()->v;
        Vecteur2D b = this->arcs[1]->debut()->v;
        Vecteur2D c = this->arcs[2]->debut()->v;

        Vecteur2D centre;
        // Les formules : https://en.wikipedia.org/wiki/Circumscribed_circle#Circumcenter_coordinates
        double d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));

        centre.x = ((a.x * a.x + a.y * a.y) * (b.y - c.y) + (b.x * b.x + b.y * b.y) * (c.y - a.y) + (c.x * c.x + c.y * c.y) * (a.y - b.y)) / d;
        centre.y = ((a.x * a.x + a.y * a.y) * (c.x - b.x) + (b.x * b.x + b.y * b.y) * (a.x - c.x) + (c.x * c.x + c.y * c.y) * (b.x - a.x)) / d;

        //Un sommet du triangle
        Vecteur2D s = this->arcs[0]->arete->debut->v;

        //Le rayon est égal à la distance entre le centre du cercle circonscrit et un des sommets du triangle
        double rayon = sqrt((s.x - centre.x) * (s.x - centre.x) + (s.y - centre.y) * (s.y - centre.y));
        return Cercle(centre, rayon);
    }

    Sommet<Vecteur2D>* sommet_oppose(ArcTU<T>* arc) {
        for (ArcTU<T> *a : this->arcs) {
            Sommet<Vecteur2D>* s = a->debut();
            Sommet<Vecteur2D>* s2 = a->fin();

            if ((s->v.x != arc->debut()->v.x && s->v.y != arc->debut()->v.y)
                && (s->v.x != arc->fin()->v.x && s->v.y != arc->fin()->v.y))
                return s;

            if ((s2->v.x != arc->debut()->v.x && s2->v.y != arc->debut()->v.y)
                && (s2->v.x != arc->fin()->v.x && s2->v.y != arc->fin()->v.y))
                return s2;
        }
        return NULL;
    }

    friend bool operator==(const Triangle<S, T>& lhs, const Triangle<S, T>& rhs) {
        return (lhs.arcs[0]->arete == rhs.arcs[0]->arete &&
            lhs.arcs[1]->arete == rhs.arcs[1]->arete &&
            lhs.arcs[2]->arete == rhs.arcs[2]->arete);
    }

};
