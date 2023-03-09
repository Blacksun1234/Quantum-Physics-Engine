#include "stdafx.h"
#include "QmFixedSpring.h"

Quantum::QmFixedSpring::QmFixedSpring(glm::vec3 *fixedpos)
{
	_restLength = 2.0f;
	_springConstant = 300.0;
	_fixedpos = fixedpos;
}

glm::vec3* Quantum::QmFixedSpring::getFixedPos() {
	return _fixedpos;
}

float Quantum::QmFixedSpring::getRestLengthFixed() {
	return _restLength;
}

float Quantum::QmFixedSpring::getSpringConstantFixed() {
	return _springConstant;
}

void Quantum::QmFixedSpring::update(QmParticle* p)
{
	glm::vec3 d = p->getPos() - *_fixedpos;
	float coeff = -(glm::length(d) - _restLength) * _springConstant;
	p->addForce(normalize(d) * coeff);
}