#pragma once
#include "QmForceGenerator.h"

namespace Quantum {

    class QmFixedSpring :
        public QmForceGenerator
    {
    public:
        QmFixedSpring(glm::vec3 *_fixedpos);
        void update(QmParticle* p);
        glm::vec3* getFixedPos();
        float getRestLengthFixed();
        float getSpringConstantFixed();
    private:
        float _restLength;
        float _springConstant;
        glm::vec3 *_fixedpos;
    };
}



