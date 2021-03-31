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
    Triangle(vector<Arc<S>> arcs): Face<S>(arcs){}

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


        double** d = new double* [3];
        d[0] = new double[3]{ a.x, a.y, 1 };
        d[1] = new double[3]{ b.x, b.y, 1 };
        d[2] = new double[3]{ c.x, c.y, 1 };
        Matrice delta = (Matrice(3, 3, d) * 2);


        double** x = new double* [3];
        x[0] = new double[3]{ pow(a.x,2) + pow(a.y,2), a.y, 1 };
        x[1] = new double[3]{ pow(b.x,2) + pow(b.y,2), b.y, 1 };
        x[2] = new double[3]{ pow(c.x,2) + pow(c.y,2), c.y, 1 };

        Matrice xO = delta.pow_m1() * Matrice(3, 3, x);

        double** y = new double* [3];
        y[0] = new double[3]{ pow(a.x,2) + pow(a.y,2), a.x, 1 };
        y[1] = new double[3]{ pow(b.x,2) + pow(b.y,2), b.x, 1 };
        y[2] = new double[3]{ pow(c.x,2) + pow(c.y,2), c.x, 1 };

        Matrice yO = delta.pow_m1() * (Matrice(3, 3, x) * -1);


        //Calcul du centre
        Vecteur2D centre = Vecteur2D();

        //Un sommet du triangle
        Vecteur2D s = this->arcs[0].arete->debut->v;

        //Le rayon est égal à la distance entre le centre du cercle circonscrit et un des sommets du triangle
        double rayon = sqrt((s.x - centre.x) * (s.x - centre.x) + (s.y - centre.y) * (s.y - centre.y));
        return Cercle(centre, rayon);
    }
};