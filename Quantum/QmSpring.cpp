#include "stdafx.h"
#include "QmSpring.h"


Quantum::QmSpring::QmSpring(QmParticle* otherbody, float *restLength, float *springConstant)
{
	_restLength = *restLength; // longueur câble au repos
	_springConstant = *springConstant; // puissance pr revenir au repos
	_otherbody = otherbody;
}

Quantum::QmParticle* Quantum::QmSpring::getOtherParticle() {

	return _otherbody;
}

float Quantum::QmSpring::getRestLength() {
	return _restLength;
}

float Quantum::QmSpring::getSpringConstant() {
	return _springConstant;
}

float getSpringConstant();


void Quantum::QmSpring::update(QmParticle* p)
{
	glm::vec3 d = p->getPos() - _otherbody -> getPos();
	float coeff = -(glm::length(d) - _restLength) * _springConstant;
	p->addForce(normalize(d) * coeff);
}

