#include "stdafx.h"
#include "QmContact.h"
#include "QmBody.h"

Quantum::QmContact::QmContact(QmBody* b1, QmBody* b2)
{
	_body1 = b1;
	_body2 = b2;
	_normal = glm::vec3(0,0,0);
	_depth = 0;
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

