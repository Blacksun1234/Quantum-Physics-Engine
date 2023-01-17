#include "stdafx.h"

#include "QmParticle.h"
#include "QmUpdater.h"

using namespace Quantum;

QmParticle::QmParticle() : position(0, 0, 0), velocity(0, 0, 0), acceleration(0, 0, 0)
{
}

QmParticle::QmParticle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc) : QmParticle()
{
	position = pos;
	velocity = vel;
	acceleration = acc;
	damping = 1.f;
	type = TYPE_PARTICLE;
}

QmParticle::~QmParticle()
{
	delete updater;
}

void QmParticle::integrate(float t)
{
	// your code here
	updater->update();
}

glm::vec3 QmParticle::getAcc()
{
	return acceleration;
}

glm::vec3 QmParticle::getVel()
{
	return velocity;
}

glm::vec3 QmParticle::getPos()
{
	return position;
}

void QmParticle::setUpdater(QmUpdater* updater)
{
	this->updater = updater;
}


