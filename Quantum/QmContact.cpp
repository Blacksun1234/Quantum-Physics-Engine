#include "stdafx.h"
#include "QmContact.h"
#include "QmBody.h"
#include "QmParticle.h"
#include "QmHalfspace.h"

Quantum::QmContact::QmContact(QmBody* b1, QmBody* b2)
{
	_body1 = b1;
	_body2 = b2;
	_normal = glm::vec3(0,0,0);
	_depth = 0;

	if (dynamic_cast<QmParticle*> (b1) && dynamic_cast<QmParticle*> (b2)) {
		float radiusSomme = ((QmParticle*)b1)->getRadius() + ((QmParticle*)b2)->getRadius();
		glm::vec3 p1 = ((QmParticle*)b1)->getPos(0);
		glm::vec3 p2 = ((QmParticle*)b2)->getPos(0);

		_depth = radiusSomme - glm::length(p2 - p1);
		_normal = glm::normalize(p2 - p1);
	}
	else if (dynamic_cast<QmParticle*> (b1) && dynamic_cast<QmHalfspace*> (b2)) {
		_normal = ((QmHalfspace*)b2)->normal;
		_depth = ((QmHalfspace*)b2)->offset + ((QmParticle*)b1)->getRadius() - glm::dot(((QmParticle*)b1)->getPos(0), ((QmHalfspace*)b2)->normal);
	}
	else if (dynamic_cast<QmHalfspace*> (b1) && dynamic_cast<QmParticle*> (b2)) {
		_normal = ((QmHalfspace*)b1)->normal;
		_depth = ((QmHalfspace*)b1)->offset + ((QmParticle*)b2)->getRadius() - glm::dot(((QmParticle*)b2)->getPos(0), ((QmHalfspace*)b1)->normal);
	}
}

Quantum::QmBody* Quantum::QmContact::getBody1()
{
	return _body1;
}

Quantum::QmBody* Quantum::QmContact::getBody2()
{
	return _body2;
}

glm::vec3 Quantum::QmContact::getNormal()
{
	return _normal;
}

float Quantum::QmContact::getDepth()
{
	return _depth;
}

void Quantum::QmContact::setNormal(glm::vec3 normal)
{
	_normal = normal;
}

void Quantum::QmContact::setDepth(float depth)
{
	_depth = depth;
}

