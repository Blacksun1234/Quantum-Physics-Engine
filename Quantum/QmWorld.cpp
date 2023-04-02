#include "stdafx.h"
#include <iostream>

#include "QmWorld.h"
#include "QmParticle.h"
#include "QmForceRegistery.h"
#include "QmForceGenerator.h"
#include "QmAABB.h"
#include "QmHalfspace.h"

using namespace Quantum;

QmWorld::QmWorld()
{
	std::cout << "Starting Quantum Physics engine." << std::endl;
	_time = 0.f;
	_ticktime = 0.f;
	gravity = glm::vec3(0, -9.81, 0);
	_delta = false;
	_isCollisionActive = false;
	_isBroadPhaseActive = true;
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
		if (dynamic_cast<QmParticle*> (b)) {
			((QmParticle*)b)->GetUpdater()->update(((QmParticle*)b)->getPos(0) + dt * ((QmParticle*)b)->getVel(0));
		}	
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
			((QmParticle*)b)->SetAcc(gravity * ((QmParticle*)b)->getMass(), i);
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
	std::vector<QmContact> contact;

	/*if (_isBroadPhaseActive) {
		for (int i = 0; i < bodies.size(); i++) {
			for (int j = i + 1; j < bodies.size(); j++) {
				if (intersect(((QmParticle*)bodies[i])->getAABB(), ((QmParticle*)bodies[j])->getAABB())) {
					contact.push_back(QmContact(bodies[i], bodies[j]));
				}
			}
		}
	}
	else {
		for (int i = 0; i < bodies.size(); i++) {
			for (int j = i + 1; j < bodies.size(); j++) {
				contact.push_back(QmContact(bodies[i], bodies[j]));
			}
		}
	}*/

	if (_isBroadPhaseActive) {
		for (int i = 0; i < bodies.size(); i++) {
			for (int j = i+1; j < bodies.size(); j++) {
				if (dynamic_cast<QmHalfspace*> (bodies[i]) || dynamic_cast<QmHalfspace*> (bodies[j])) {
					contact.push_back(QmContact(bodies[i], bodies[j]));
				}
				else if (intersect(((QmParticle*)bodies[i])->getAABB(), ((QmParticle*)bodies[j])->getAABB())) {
					contact.push_back(QmContact(bodies[i], bodies[j]));
				}
			}
		}
	}
	else {
		for (int i = 0; i < bodies.size(); i++) {
			for (int j = i + 1; j < bodies.size(); j++) {
				contact.push_back(QmContact(bodies[i], bodies[j]));
			}
		}
	}
	return contact;
}

std::vector<QmContact> Quantum::QmWorld::narrowphase(std::vector<QmContact> contact)
{
	//optimize the bodies to find which collision it is
	/*if(p1 p2)
	else if (p1 hs)*/

	std::vector<QmContact> TrueContact;
	for (QmContact contact : contact) {

		if (contact.getDepth() >= 0) {
			//add contact to the list
			TrueContact.push_back(contact);
		}
	}
	//std::cout << "TrueContact Size :" << TrueContact.size() << std::endl;
	return TrueContact;
}

void Quantum::QmWorld::resolve(std::vector<QmContact> TrueContact)
{
	for (QmContact TrueContact : TrueContact) {

		if (dynamic_cast<QmHalfspace*> (TrueContact.getBody1()) && dynamic_cast<QmParticle*> (TrueContact.getBody2())) {
			float depth = TrueContact.getDepth();
			((QmParticle*)TrueContact.getBody2())->SetPos(depth * TrueContact.getNormal(), 0);

			glm::vec3 V2 = ((QmParticle*)TrueContact.getBody2())->getVel(0);
			glm::vec3 v2perpendiculaire = (glm::dot(TrueContact.getNormal(), V2)) * TrueContact.getNormal();
			glm::vec3 v2parallele = V2 - v2perpendiculaire;
			glm::vec3 v2 = -v2perpendiculaire;
			glm::vec3 vel2 = v2 + v2parallele;
			((QmParticle*)TrueContact.getBody2())->SetVel(vel2 * 0.99f, 0);

		}
		else if (dynamic_cast<QmParticle*> (TrueContact.getBody1()) && dynamic_cast<QmHalfspace*> (TrueContact.getBody2())) {
			float depth = TrueContact.getDepth();
			((QmParticle*)TrueContact.getBody1())->SetPos(depth * TrueContact.getNormal(), 0);

			glm::vec3 V1 = ((QmParticle*)TrueContact.getBody1())->getVel(0);
			glm::vec3 v1perpendiculaire = (glm::dot(TrueContact.getNormal(), V1)) * TrueContact.getNormal();
			glm::vec3 v1parallele = V1 - v1perpendiculaire;
			glm::vec3 v1 = -v1perpendiculaire;
			glm::vec3 vel1 = v1 + v1parallele;
			((QmParticle*)TrueContact.getBody1())->SetVel(vel1 * 0.99f, 0);
		}
		else if(dynamic_cast<QmParticle*> (TrueContact.getBody1()) && dynamic_cast<QmParticle*> (TrueContact.getBody2())){

			//move the particules to the correct position
			float depth = TrueContact.getDepth();
			float m1 = ((QmParticle*)TrueContact.getBody1())->getMass();
			float m2 = ((QmParticle*)TrueContact.getBody2())->getMass();
			glm::vec3 pos1 = (depth * m2 / (m1 + m2)) * TrueContact.getNormal();
			glm::vec3 pos2 = (depth * m1 / (m1 + m2)) * TrueContact.getNormal();

			((QmParticle*)TrueContact.getBody1())->SetPos(-pos1, 0);
			((QmParticle*)TrueContact.getBody2())->SetPos(pos2, 0);

			glm::vec3 V1 = ((QmParticle*)TrueContact.getBody1())->getVel(0);
			glm::vec3 V2 = ((QmParticle*)TrueContact.getBody2())->getVel(0);
			//(dot pro entre normal collision, velocité(0)) fois la normal collision

			glm::vec3 v1perpendiculaire = (glm::dot(TrueContact.getNormal(), V1)) * TrueContact.getNormal();
			glm::vec3 v2perpendiculaire = (glm::dot(TrueContact.getNormal(), V2)) * TrueContact.getNormal();

			glm::vec3 v1parallele = V1 - v1perpendiculaire;
			glm::vec3 v2parallele = V2 - v2perpendiculaire;

			glm::vec3 v1 = ((m1 - m2) / (m1 + m2)) * v1perpendiculaire + ((2.0f * m2) / (m1 + m2)) * v2perpendiculaire;
			glm::vec3 v2 = ((2.0f * m1) / (m1 + m2)) * v1perpendiculaire + ((m2 - m1) / (m1 + m2)) * v2perpendiculaire;

			glm::vec3 vel1 = v1 + v1parallele;
			glm::vec3 vel2 = v2 + v2parallele;

			((QmParticle*)TrueContact.getBody1())->SetVel(vel1 * 0.99f, 0);
			((QmParticle*)TrueContact.getBody2())->SetVel(vel2 * 0.99f, 0);
		}
		
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

