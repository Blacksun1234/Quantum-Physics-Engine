#pragma once
#include "QmForceGenerator.h"


namespace Quantum {

    class QmParticle;

    class QmSpring :
        virtual public QmForceGenerator
    {
    public:
        QmSpring(float, float, QmParticle*);
        void update(QmParticle* p);
    private:
        float _restLength;
        float _springConstant;
        QmParticle* _otherbody;
    };

}
