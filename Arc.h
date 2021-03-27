#pragma once
#include "Arete.h"

/**
* T est la nature de l'information port�e par un sommet et
* S est la nature de l'information port�e par une ar�te
*/
template <class S, class T>
class Arc : public Arete<S, T>{
public:
	int sens;
	public Arc(Arete arete, int sens) :super(arete), sens(sens){}
};