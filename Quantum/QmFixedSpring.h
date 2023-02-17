#pragma once
#include "QmForceGenerator.h"

namespace Quantum {

    class QmFixedSpring :
        public QmForceGenerator
    {
    public:
        QmFixedSpring(float, float, glm::vec3 _fixedpos, QmParticle*);
        void update(QmParticle* p);
    private:
        float _restLength;
        float _springConstant;
        glm::vec3 _fixedpos;
        QmParticle* _otherbody;
    };
}



