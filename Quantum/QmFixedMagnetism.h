#pragma once
#include "QmForceGenerator.h"

namespace Quantum {

    class QmFixedMagnetism :
        virtual public QmForceGenerator
    {
    public:
        QmFixedMagnetism(float _K, float eps, glm::vec3 *fixedpos, float *fixedCharge);
        void update(QmParticle* p);
    private:
        float _K;
        float _eps;
        glm::vec3 *_fixedPos;
        float* _fixedCharge;
    };
}
