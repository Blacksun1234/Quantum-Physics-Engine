#pragma once
#include "QmForceGenerator.h"

namespace Quantum {
    class QmParticule;

    class QmMagnetism :
        public QmForceGenerator
    {
    public:
        QmMagnetism(float _K, float eps, QmParticle* _otherbody);
        void update(QmParticule* p) {};
    private:
        float _K;
        float eps;
        QmParticle* _otherbody;
    };

}

