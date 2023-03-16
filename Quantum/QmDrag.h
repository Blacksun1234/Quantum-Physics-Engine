#pragma once
#include "QmForceGenerator.h"

namespace Quantum {

    class QmParticle;

    class QmDrag :
        virtual public QmForceGenerator
    {
    public:
        QmDrag(float K1, float K2);
        void update(QmParticle* p, int);

    private:
        float K1 = 1;
        float K2 = 2;
    };
}



