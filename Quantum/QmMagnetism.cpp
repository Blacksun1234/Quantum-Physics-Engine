#include "stdafx.h"
#include "QmMagnetism.h"
#include <glm/glm.hpp>
#include <glm/geometric.hpp>

Quantum::QmMagnetism::QmMagnetism(float K, float eps, QmParticle* otherbody) {
	_K = K;
    _eps = eps;
    _otherbody = otherbody;
}

void Quantum::QmMagnetism::update(QmParticle* p, int i) {
    glm::vec3 d = p->getPos(i) - _otherbody->getPos(i);
    float coeff = _K * (p->getCharge() * _otherbody->getCharge());
    p->addForce(normalize(d) * coeff / float(pow(glm::length(d), 2.0) + _eps), i);
}