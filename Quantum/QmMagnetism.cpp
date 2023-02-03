#include "stdafx.h"
#include "QmMagnetism.h"
#include <glm/glm.hpp>
#include <glm/geometric.hpp>

Quantum::QmMagnetism::QmMagnetism(float _K, float eps, QmParticle* _otherbody) {
	K = _K;

}

void Quantum::QmMagnetism::update(QmParticule* p) {
	glm::vec3 d = p->pos
};