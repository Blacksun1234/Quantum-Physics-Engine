#include "stdafx.h"
#include "QmFixedSpring.h"

Quantum::QmFixedSpring::QmFixedSpring(float restLength, float springConstant, glm::vec3 fixedpos, QmParticle* otherbody)
{
	_restLength = restLength;
	_springConstant = springConstant;
	_otherbody = otherbody;
	_fixedpos = fixedpos;
}

void Quantum::QmFixedSpring::update(QmParticle* p)
{
	glm::vec3 d = p->getPos() - _fixedpos;
	float coeff = -(glm::length(d) - _restLength) * _springConstant;
	p->addForce(normalize(d) * coeff);
}