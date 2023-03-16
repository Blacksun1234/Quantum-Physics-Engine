#pragma once
#include "QmForceGenerator.h"

namespace Quantum {

    class QmFixedSpring :
        public QmForceGenerator
    {
    public:
        QmFixedSpring(glm::vec3 *_fixedpos, float *restLength, float *springConstant);
        void update(QmParticle* p, int);
        glm::vec3* getFixedPos();
        float getRestLengthFixed();
        float getSpringConstantFixed();
    private:
        float _restLength;
        float _springConstant;
        glm::vec3 *_fixedpos;
    };
}



