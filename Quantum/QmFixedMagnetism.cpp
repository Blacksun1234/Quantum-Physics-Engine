#include "stdafx.h"
#include "QmFixedMagnetism.h"

Quantum::QmFixedMagnetism::QmFixedMagnetism(float K, float eps, glm::vec3 *fixedPos, float *fixedCharge)
{
	_K = K;
	_eps = eps;
	_fixedPos = fixedPos;
	_fixedCharge = fixedCharge;
}

void Quantum::QmFixedMagnetism::update(QmParticle* p, int i)
{
	glm::vec3 d = p->getPos(i) - *_fixedPos;
	float coeff = _K * (p->getCharge() * *_fixedCharge);
	p->addForce(normalize(d) * coeff / float(pow(glm::length(d), 2.0) + _eps), i);
}
