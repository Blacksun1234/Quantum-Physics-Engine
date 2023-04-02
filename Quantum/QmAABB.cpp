#include "stdafx.h"
#include "QmAABB.h"
#include "QmParticle.h"
#include "QmHalfspace.h"
#include <iostream>


Quantum::QmAABB::QmAABB(QmBody* b)
{   
    if (dynamic_cast<QmParticle*> (b)) {
        QmParticle* p = (QmParticle*)b;
        min = p->getPos(0) - p->getRadius();
        max = p->getPos(0) + p->getRadius();
    }
    else if (dynamic_cast<QmHalfspace*> (b)) {
        QmHalfspace* hs = (QmHalfspace*)b;

        glm::vec3 v1(1.0f, 0.0f, 0.0f);

        //on test si la normal est egal au vecteur v1, si oui, on prends un nouveau v1
        if (glm::length(hs->normal - v1) < 0.001f || glm::length(hs->normal + v1) < 0.001f)
            v1 = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec3 v2 = glm::normalize(glm::cross(hs->normal, v1));
        glm::vec3 v3 = glm::normalize(glm::cross(hs->normal, v2));

        min = (FLT_MAX / 10.0f) * v2 + (FLT_MAX / 10.0f) * v3;
        max = -(FLT_MAX / 10.0f) * v2 - (FLT_MAX / 10.0f) * v3;
    }
   /* std::cout << "min " << min.x << " " << min.y << " " << min.z << " " << std::endl;
    std::cout << "max " << max.x << " " << max.y << " " << max.z << " " << std::endl;*/
    //std::cout << p->getRadius();
}

glm::vec3 Quantum::QmAABB::getMin()
{
    return min;
}

glm::vec3 Quantum::QmAABB::getMax()
{
    return max;
}