#include "stdafx.h"
#include "QmDrag.h"
#include <glm/geometric.hpp>


Quantum::QmDrag::QmDrag(float _K1, float _K2) {
	K1 = _K1;
	K2 = _K2;
}

void Quantum::QmDrag::update(QmParticle* p)
{
	float coeff = (- 1.0f) * (K1 * glm::length((p->getVel())) + K2 * glm::length((p->getVel()) * (p->getVel())));
	p->addForce(glm::normalize(p->getVel()) * coeff);
}
