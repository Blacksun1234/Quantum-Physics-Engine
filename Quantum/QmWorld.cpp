#include "stdafx.h"
#include <iostream>

#include "QmWorld.h"
#include "QmParticle.h"
#include "QmForceRegistery.h"
#include "QmForceGenerator.h"

using namespace Quantum;

QmWorld::QmWorld()
{
	std::cout << "Starting Quantum Physics engine." << std::endl;
	time = 0.f;
	//isGravityActive = true;
	gravity = glm::vec3(0, -9.81, 0);
}

QmWorld::~QmWorld()
{

}

void QmWorld::SetGravity(bool isGravityActive) {
	_isGravityActive = isGravityActive;
}

void QmWorld::simulate(float t)
{
	clear(); // clear of particules to set acc to 0
	applyGravity(); //apply gravity to all bodies vector
	updateForces(); // update forces to each iteration
	integrate(t);
}

void QmWorld::integrate(float t)
{
	time += t;
	for (QmBody* b : bodies)
		b->integrate(t);
}

void QmWorld::applyGravity() {
	if (_isGravityActive) {
		for (QmBody* b : bodies) {
			b->SetAcc(gravity);
		}	
	}
}

void QmWorld::updateForces() {
	for (QmForceRegistery*  fr: forcesRegistries) {
		fr->fg->update(fr->p);
	}
}

void QmWorld::addBody(QmBody* b)
{
	bodies.push_back(b);
}

void QmWorld::addForceRegistery(QmForceRegistery* f) {
	forcesRegistries.push_back(f);
}

std::vector<QmBody*> QmWorld::getBodies()
{
	return bodies;
}

void QmWorld::clear()
{

	for (QmBody* b : bodies)
	{
		b->clearParticle();
	}
}

void QmWorld::clearScene() {
	for (QmBody* b : bodies)
	{
		delete b;
	}
	for (QmForceRegistery* f : forcesRegistries)
	{
		delete f;
	}
	bodies.clear();
	forcesRegistries.clear();
}

