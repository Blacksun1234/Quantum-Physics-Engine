#include "stdafx.h"
#include <iostream>

#include "QmWorld.h"
#include "QmParticle.h"

using namespace Quantum;

QmWorld::QmWorld()
{
	std::cout << "Starting Quantum Physics engine." << std::endl;
	time = 0.f;
}

QmWorld::~QmWorld()
{

}

void QmWorld::simulate(float t)
{
	integrate(t);
}


void QmWorld::integrate(float t)
{
	time += t;
	for (QmBody* b : bodies)
		b->integrate(t);
}

void QmWorld::addBody(QmBody* b)
{
	bodies.push_back(b);
}

std::vector<QmBody*> QmWorld::getBodies()
{
	return bodies;
}

void QmWorld::clear()
{

	for (QmBody* b : bodies)
	{
		delete b;
	}
	bodies.clear();
}

