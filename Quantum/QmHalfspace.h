#pragma once
#include "QmBody.h"

namespace Quantum {
    class QmHalfspace :
        public QmBody
    {
    public:
        QmHalfspace (glm::vec3 n, float o);
        QmAABB getAABB();
        glm::vec3 normal;
        float offset;
    };
}