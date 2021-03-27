#pragma once
#include <vector>
#include "Sommet.h"
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
};