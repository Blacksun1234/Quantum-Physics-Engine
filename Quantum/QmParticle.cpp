#include "stdafx.h"
#include <iostream>
#include "QmParticle.h"
#include "QmUpdater.h"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

using namespace Quantum;

QmParticle::QmParticle() : position(0, 0, 0), velocity(0, 0, 0), acceleration(0, 0, 0)
{
}

QmParticle::QmParticle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float invMass) : QmParticle()
{
	position = pos;
	velocity = vel;
	acceleration = acc;
	invMass = invMass;
	damping = 1.f;
	type = TYPE_PARTICLE;
}

QmParticle::~QmParticle()
{
	delete updater;
}

void QmParticle::integrate(float t)
{
	//acc += force accumulator * InvMass
	acceleration += forceAccumulator + invMass;
	// your code here
	position = position + t * velocity;
	velocity = velocity + t * acceleration;
	if (updater != NULL) {
		updater->update(position);
	}
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

void Quantum::QmParticle::SetAcc(glm::vec3 acc)
{
	acceleration += acc;
}

void QmParticle::setUpdater(QmUpdater* updater)
{
	this->updater = updater;
}

void Quantum::QmParticle::addForce(glm::vec3 f)
{
	//std::cout << glm::to_string(f) << std::endl;
	forceAccumulator = forceAccumulator + f;
}

void Quantum::QmParticle::clearParticle() {
	acceleration = glm::vec3(0);
	forceAccumulator = glm::vec3(0);
}