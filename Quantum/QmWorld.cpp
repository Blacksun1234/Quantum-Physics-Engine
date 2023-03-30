#include "stdafx.h"
#include <iostream>

#include "QmWorld.h"
#include "QmParticle.h"
#include "QmForceRegistery.h"
#include "QmForceGenerator.h"
#include "QmAABB.h"

using namespace Quantum;

QmWorld::QmWorld()
{
	std::cout << "Starting Quantum Physics engine." << std::endl;
	_time = 0.f;
	_ticktime = 0.f;
	gravity = glm::vec3(0, -9.81, 0);
	_delta = false;
}

QmWorld::~QmWorld()
{

}

void QmWorld::SetGravity(bool isGravityActive) {
	_isGravityActive = isGravityActive;
}

void Quantum::QmWorld::SetCollision(bool isCollisionActive){
	_isCollisionActive = isCollisionActive;
}

void Quantum::QmWorld::SetBroadPhase(bool isBroadPhaseActive)
{
	_isBroadPhaseActive = isBroadPhaseActive;
}

void QmWorld::SetDelta() {
	if (_delta) {
		_delta = false;
	}
	else {
		_delta = true;
	}
}

bool QmWorld::GetDelta() {
	return _delta;
}

float QmWorld::tickRK4(float t) {
	clear();// clear of particules to set acc to 0

	applyGravity(0);
	updateForces(0);
	integrate(t / 2.0f, 1);

	applyGravity(1);
	updateForces(1);
	integrate(t / 2.0f, 2);

	applyGravity(2);
	updateForces(2);
	integrate(t, 3);

	applyGravity(3);
	updateForces(3);
	integrate_RK4(t);

	_ticktime += t;
	return _time - _ticktime;
}

float QmWorld::tick(float t) {
	clear(); // clear of particules to set acc to 0
	applyGravity(0); //apply gravity to all bodies vector
	updateForces(0); // update forces to each iteration
	integrate(t,0);
	_ticktime += t;
	return _time - _ticktime;
}

void QmWorld::interpolate(float dt) {
	for (QmBody* b : bodies) {
		((QmParticle*)b)->GetUpdater()->update(((QmParticle*)b)->getPos(0) + dt * ((QmParticle*)b)->getVel(0));
	}
}

void QmWorld::simulate(float t)
{
	_time += t;
	float dt;
	bool useDELTA = _delta;
	float DELTA = 0.01f;
	dt = _time - _ticktime;
	if (useDELTA) { // deterministic framerate-independent simulation
		while (dt >= DELTA) {
			if (numericalIntegrator == 2) {
				dt = tickRK4(DELTA);
			}
			else {
				dt = tick(DELTA);
			}
			//printf("-\n");
		}
		//printf("------------\n");
		interpolate(dt);

		if (_isCollisionActive) {
			resolve(narrowphase(broadphase()));
		}
	}
	else { // old fashioned all-frame non-deterministic simulation
		if (numericalIntegrator == 2) {
			tickRK4(t);
		}
		else {
			tick(t);
		}
		interpolate(0.0f);
		if (_isCollisionActive) {
			resolve(narrowphase(broadphase()));
		}
	}
}

void QmWorld::integrate(float t, int i)
{
	if (numericalIntegrator == 0) {
		for (QmBody* b : bodies)
			b->integrateExplicit(t, i);
	}
	else if (numericalIntegrator == 1) {
		for (QmBody* b : bodies)
			b->integrateSemiExplicit(t, i);
	}
	else if (numericalIntegrator == 2) {
		for (QmBody* b : bodies)
			b->integrateRK4(t, i);
	}
}

void QmWorld::integrate_RK4(float t) {
	for (QmBody* b : bodies)
		b->integrate_RK4(t);
}

void QmWorld::applyGravity(int i) {
	if (_isGravityActive) {
		for (QmBody* b : bodies) {
			b->SetAcc(gravity, i);
		}	
	}
}

void QmWorld::updateForces(int i) {
	for (QmForceRegistery*  fr: forcesRegistries) {
		fr->fg->update(fr->p, i);
	}
}

void QmWorld::addBody(QmBody* b)
{
	bodies.push_back(b);
}

void QmWorld::addForceRegistery(QmForceRegistery* f) {
	forcesRegistries.push_back(f);
}

std::vector <QmForceRegistery*> QmWorld::getForcesRegistery() {
	return forcesRegistries;
}

std::vector<QmContact> Quantum::QmWorld::broadphase()
{
	//std::cout << "broadphase" << std::endl;
	std::vector<QmContact> contact;
	/*for (QmBody* b : bodies) {
		for (QmBody* b2 : bodies) {
			if (b2 != b) {
				if ( intersect(((QmParticle*)b) -> getAABB(), ((QmParticle*)b2) -> getAABB()) ) {
					contact.push_back(QmContact(b, b2));
					std::cout << "contact" << std::endl;
				}
			}
		}
	}*/

	for (int i = 0; i < bodies.size(); i++) {
		for (int j = i+1; j < bodies.size(); j++) {

			//std::cout << (intersect( ((QmParticle*)bodies[i])->getAABB(), ((QmParticle*)bodies[j])->getAABB()) ? "True" : "False") << std::endl;

			if (intersect(((QmParticle*)bodies[i])->getAABB(), ((QmParticle*)bodies[j])->getAABB())) {
				contact.push_back(QmContact(bodies[i], bodies[j]));
				//std::cout << "contact" << std::endl;
			}
		}
	}

	//std::cout << "Contact Size :" << contact.size() << std::endl;
	return contact;
}

std::vector<QmContact> Quantum::QmWorld::narrowphase(std::vector<QmContact> contact)
{
	std::vector<QmContact> TrueContact;
	for (QmContact contact : contact) {
		float radiusSomme = ((QmParticle*)contact.getBody1())->getRadius() + ((QmParticle*)contact.getBody2())->getRadius();
		glm::vec3 p1 = ((QmParticle*)contact.getBody1())->getPos(0);
		glm::vec3 p2 = ((QmParticle*)contact.getBody2())->getPos(0);
		float depth = radiusSomme - glm::length(p2 - p1);
		if (depth < 0) {
			TrueContact.push_back(contact);
			//print normal and depth
			contact.setNormal(glm::normalize(p2 - p1));
			contact.setDepth(depth);
		}
	}
	std::cout << "TrueContact Size :" << TrueContact.size() << std::endl;
	return TrueContact;
}

void Quantum::QmWorld::resolve(std::vector<QmContact> TrueContact)
{
	for (QmContact TrueContact : TrueContact) {
		//move the particules to the correct position
		float depth = TrueContact.getDepth();
		float m1 = ((QmParticle*)TrueContact.getBody1())->getMass();
		float m2 = ((QmParticle*)TrueContact.getBody2())->getMass();
		float pos1 = depth * m2 / (m2 + m2);
		float pos2 = depth * m1 / (m2 + m1);
		((QmParticle*)TrueContact.getBody1())->SetPosMove(pos1);
		((QmParticle*)TrueContact.getBody2())->SetPosMove(pos2);

		//compute new velocities
		//v1|| vel - v1 perpendiculaire
		
		glm::vec3 V1 = ((QmParticle*)TrueContact.getBody1())->getVel(0);
		glm::vec3 V2 = ((QmParticle*)TrueContact.getBody2())->getVel(0);
		//(dot pro entre normal collision, velocité(0)) fois la normal collision
		glm::vec3 v1perpendiculaire = (glm::dot(TrueContact.getNormal(),V1))* TrueContact.getNormal();
		glm::vec3 v2perpendiculaire = (glm::dot(TrueContact.getNormal(), V2)) * TrueContact.getNormal();

		glm::vec3 v1 = ((m1 - m2) / m1 + m2) * v1perpendiculaire + ((2 * m2) / m1 + m2) * v2perpendiculaire;
		glm::vec3 v2 = ((2 * m1) / m1 + m2) * v1perpendiculaire + ((m2 - m1) / m1 + m2) * v2perpendiculaire;

		glm::vec3 v1parallele = ((QmParticle*)TrueContact.getBody1())->getVel(0) - v1perpendiculaire;
		glm::vec3 v2parallele = ((QmParticle*)TrueContact.getBody2())->getVel(0) - v2perpendiculaire;

		((QmParticle*)TrueContact.getBody1())->SetVel((v1 + v1parallele) * 0.99f, 0);
		((QmParticle*)TrueContact.getBody2())->SetVel((v2 + v2parallele) * 0.99f, 0);
	}
}

bool Quantum::QmWorld::intersect(QmAABB a, QmAABB b)
{
	return
		(a.getMin().x <= b.getMax().x && a.getMax().x >= b.getMin().x) &&
		(a.getMin().y <= b.getMax().y && a.getMax().y >= b.getMin().y) &&
		(a.getMin().z <= b.getMax().z && a.getMax().z >= b.getMin().z);
}

std::vector<QmBody*> QmWorld::getBodies()
{
	return bodies;
}

int QmWorld::getNumericalIntegrator() {
	return numericalIntegrator;
}

void QmWorld::changeNumericalIntegrator() {
	numericalIntegrator++;
	numericalIntegrator %= 3;
}

bool Quantum::QmWorld::getGravityIsActive()
{
	return _isGravityActive;
}

bool Quantum::QmWorld::getCollisionIsActive()
{
	return _isCollisionActive;
}

bool Quantum::QmWorld::getBroadPhaseIsActive()
{
	return _isBroadPhaseActive;
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

