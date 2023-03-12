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

QmParticle::QmParticle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float Mass, float charge) : QmParticle()
{
	position = pos;
	velocity = vel;
	acceleration = acc;
	invMass = 1 / Mass;
	//damping = (charge == 0.0f) ? .90f : 1.0f;
	damping = 0.99f;
	type = TYPE_PARTICLE;
	_charge = charge;
}

QmParticle::~QmParticle()
{
	delete updater;
}

void QmParticle::integrateExplicit(float t)
{
	acceleration += forceAccumulator * invMass;
	position += t * velocity;
	velocity += t * acceleration;
	velocity *= damping;
	if (updater != NULL) {
		updater->update(position);
	}
}

void QmParticle::integrateSemiExplicit(float t)
{
	acceleration += forceAccumulator * invMass;
	velocity += t * acceleration;
	position += t * velocity;
	velocity *= damping;
	if (updater != NULL) {
		updater->update(position);
	}
}

void QmParticle::integrateRK4(float t)
{
	
}

QmUpdater* QmParticle::GetUpdater() {
	return updater;
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

float QmParticle::getCharge() {
	return _charge;
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
	acceleration = glm::vec3(0,0,0);
	forceAccumulator = glm::vec3(0,0,0);
}
