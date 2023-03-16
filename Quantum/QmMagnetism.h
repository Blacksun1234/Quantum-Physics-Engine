#pragma once
#include "QmForceGenerator.h"

namespace Quantum {

    class QmParticle;

    class QmMagnetism :
        virtual public QmForceGenerator
    {
    public:
        QmMagnetism(float _K, float eps, QmParticle* _otherbody);
        void update(QmParticle* p, int);
    private:
        float _K;
        float _eps;
        QmParticle* _otherbody;
    };

}

