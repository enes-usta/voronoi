#pragma once
#include "Arete.h"

/**
* T est la nature de l'information portée par un sommet et
* S est la nature de l'information portée par une arête
*/
template <class S, class T>
class Arc : public Arete<S, T>{
public:
	int sens;
	public Arc(Arete arete, int sens) :super(arete), sens(sens){}
};