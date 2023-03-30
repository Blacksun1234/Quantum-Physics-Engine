#include "stdafx.h"
#include <iostream>
#include "QmParticle.h"
#include "QmUpdater.h"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include "QmAABB.h"

using namespace Quantum;

QmParticle::QmParticle()
{
	_position[0] = glm::vec3(0,0,0);
	_velocity[0] = glm::vec3(0, 0, 0);
	_acceleration[0] = glm::vec3(0, 0, 0);
	_forceAccumulator[0] = glm::vec3(0, 0, 0);
}

QmParticle::QmParticle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float Mass, float charge, float radius) : QmParticle()
{
	_position[0] = pos;
	_velocity[0] = vel;
	_acceleration[0] = acc;
	_forceAccumulator[0] = glm::vec3(0, 0, 0);
	invMass = 1 / Mass;
	_mass = Mass;
	//damping = (charge == 0.0f) ? .90f : 1.0f;
	damping = 0.99f;
	type = TYPE_PARTICLE;
	_charge = charge;
	_radius = radius;
}

QmParticle::~QmParticle()
{
	delete updater;
}

void QmParticle::integrateExplicit(float t, int i)
{
	_acceleration[i] += _forceAccumulator[i] * invMass;
	_position[i] += t * _velocity[i];
	_velocity[i] += t * _acceleration[i];
	_velocity[i] *= damping;
	if (updater != NULL) {
		updater->update(_position[i]);
	}
}

void QmParticle::integrateSemiExplicit(float t, int i)
{
	_acceleration[i] += _forceAccumulator[i] * invMass;
	_velocity[i] += t * _acceleration[i];
	_position[i] += t * _velocity[i];
	_velocity[i] *= damping;
	if (updater != NULL) {
		updater->update(_position[i]);
	}
}

void QmParticle::integrateRK4(float t, int i)
{
	_acceleration[i - 1] += _forceAccumulator[i - 1] * invMass;
	_position[i] = _position[0] + t * _velocity[i - 1];
	_velocity[i] = _velocity[0] + t * _acceleration[i - 1];
}

void QmParticle::integrate_RK4(float t) {
	_acceleration[3] += _forceAccumulator[3] * invMass;
	_position[0] += t * ((_velocity[0] + 2.0f * _velocity[1] + 2.0f * _velocity[2] + _velocity[3]) / 6.0f);
	_velocity[0] += t * ((_acceleration[0] + 2.0f * _acceleration[1] + 2.0f * _acceleration[2] + _acceleration[3]) / 6.0f);

	_velocity[0] *= damping;

	if (updater != NULL) {
		updater->update(_position[0]);
	}
}

QmUpdater* QmParticle::GetUpdater() {
	return updater;
}

glm::vec3 QmParticle::getAcc(int i)
{
	return _acceleration[i];
}

glm::vec3 QmParticle::getVel(int i)
{
	return _velocity[i];
}

glm::vec3 QmParticle::getPos(int i)
{
	return _position[i];
}

float Quantum::QmParticle::getRadius()
{
	return _radius;
}

float QmParticle::getCharge() {
	return _charge;
}

float Quantum::QmParticle::getMass()
{
	return _mass;
}

void Quantum::QmParticle::SetAcc(glm::vec3 acc, int i)
{
	_acceleration[i] += acc;
}

void Quantum::QmParticle::SetPos(glm::vec3 pos, int i)
{
	_position[i] += pos;
}

void Quantum::QmParticle::SetVel(glm::vec3 vel, int i)
{
	_velocity[i] = vel;
}

void Quantum::QmParticle::SetPosMove(float pos)
{
	_position[0] += pos;
}

void QmParticle::setUpdater(QmUpdater* updater)
{
	this->updater = updater;
}

void Quantum::QmParticle::addForce(glm::vec3 f, int i)
{
	_forceAccumulator[i] = _forceAccumulator[i] + f;
}

void Quantum::QmParticle::clearParticle() {
	_acceleration[0] = glm::vec3(0,0,0);
	_acceleration[1] = glm::vec3(0,0,0);
	_acceleration[2] = glm::vec3(0,0,0);
	_acceleration[3] = glm::vec3(0,0,0);
	_forceAccumulator[0] = glm::vec3(0,0,0);
	_forceAccumulator[1] = glm::vec3(0,0,0);
	_forceAccumulator[2] = glm::vec3(0,0,0);
	_forceAccumulator[3] = glm::vec3(0,0,0);
}

QmAABB Quantum::QmParticle::getAABB()
{
	return QmAABB(this);
}
