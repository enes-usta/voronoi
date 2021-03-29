#pragma once
#include <vector>
#include "Sommet.h"
#include "Cercle.h"
#include "Arc.h"
#include "Face.h"

/**
* T est la nature de l'information portée par un sommet et
* S est la nature de l'information portée par une arête
*/
template <class S, class T>
class Triangle : public Face {
public:
    vector<Arc<S, T>> arcs;
    public Triangle(vector<Arc<S, T>> arcs) :{
        if (arcs->size != 3)
            throw "Un cercle est compos� de 3 arcs";
    }

    public Triangle(Arc<S, T> a1, Arc<S, T> a2, Arc<S, T> a3) : {
        arcs.push_back(a1);
        arcs.push_back(a2);
        arcs.push_back(a3);
    }

    /**
    * Retourne le cercle circonscrit de ce triangle
    */
    public Cercle<T> cercle_circonscrit() {
        /*
        Sommet<T> centre = ...;
        A COMPLETER
        
        */
        //Le rayon est égal à la distance entre le centre du cercle circonscrit et un des sommets du triangle
        double rayon = sqrt((this.arcs[0].x - s.x) * (this.arcs[0].x - s.x) + (this.arcs[0].y - s.y) * (this.arcs[0].y - s.y));
        return Cercle<T>(centre, rayon)
    }
};