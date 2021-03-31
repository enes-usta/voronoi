#pragma once
#include <vector>
#include "Sommet.h"
#include "Arc.h"
#include "Face.h"
#include "Matrice.h"
#include "Vecteur2D.h"

/**
* T est la nature de l'information portée par un sommet et
* S est la nature de l'information portée par une arête
*/
template <class S, class T>
class Triangle : public Face {
public:
    public Triangle(vector<Arc<S, T>> arcs){
        super(arcs);
    }

    public Triangle(Arc<S, T> a1, Arc<S, T> a2, Arc<S, T> a3) : {
        vector<Arc<S, T>> a = vector<Arc<S, T>>();
        a.push_back(a1);
        a.push_back(a2);
        a.push_back(a3);
        super(a);
    }

    /**
    * Retourne le cercle circonscrit de ce triangle
    */
    public Cercle<T> cercle_circonscrit() {
        //Calcul du centre
        Sommet<T> centre = new Sommet<T>();

        Vecteur2D a = arcs[0].arete->debut.v;
        Vecteur2D b = arcs[1].arete->debut.v;
        Vecteur2D c = arcs[2].arete->debut.v;


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

        Matrice yO = delta.pow_m1() * Matrice(3, 3, x) * -1;


        //Le rayon est égal à la distance entre le centre du cercle circonscrit et un des sommets du triangle
        double rayon = sqrt((arcs[0].x - s.x) * (arcs[0].x - s.x) + (arcs[0].y - s.y) * (arcs[0].y - s.y));
        return Cercle<T>(centre, rayon)
    }
};