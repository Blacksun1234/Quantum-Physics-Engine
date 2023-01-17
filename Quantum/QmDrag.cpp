#include "stdafx.h"
#include "QmDrag.h"
#include <glm/geometric.hpp>
#include "QmParticle.h"

void Quantum::QmDrag::update(QmParticle* p)
{
	float coeff = (- 1.0f) * (K1 * glm::length((p->getVel())) + K2 * glm::length((p->getVel()) * (p->getVel())));
	p->addForce(glm::normalize(p->getVel() * coeff));
}
