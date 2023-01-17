#pragma once
#include "QmForceGenerator.h"

namespace Quantum {

    class QmParticle;

    class QmDrag :
        public QmForceGenerator
    {
    public:
        void update(QmParticle* p);

    private:
        float K1 = 1;
        float K2 = 2;
    };
}



