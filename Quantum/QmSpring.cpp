#include "stdafx.h"
#include "QmSpring.h"

Quantum::QmSpring::QmSpring(float restLength, float springConstant, QmParticle* otherbody)
{
	_restLength = restLength;
	_springConstant = springConstant;
	_otherbody = otherbody;
}

void Quantum::QmSpring::update(QmParticle* p)
{
	glm::vec3 d = p->getPos() - _otherbody -> getPos();
	float coeff = -(glm::length(d) - _restLength) * _springConstant;
	p->addForce(normalize(d) * coeff);
}
