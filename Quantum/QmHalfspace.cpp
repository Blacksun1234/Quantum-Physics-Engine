#include "stdafx.h"
#include "QmHalfspace.h"

Quantum::QmHalfspace::QmHalfspace(glm::vec3 n, float o) {
	normal = n;
	offset = o;
}

QmAABB Quantum::QmHalfspace::getAABB()
{
	return QmAABB(this);
}


