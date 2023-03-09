#pragma once
#include "QmForceGenerator.h"
#include "QmParticle.h"


namespace Quantum {

    class QmSpring :
        public QmForceGenerator
    {
    public:
        QmSpring(QmParticle*);
        void update(QmParticle* p);
        QmParticle* getOtherParticle();
        float getRestLength();
        float getSpringConstant();
    private:
        float _restLength;
        float _springConstant;
        QmParticle* _otherbody;
    };

}
