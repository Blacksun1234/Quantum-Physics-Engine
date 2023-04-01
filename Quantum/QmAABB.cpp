#include "stdafx.h"
#include "QmAABB.h"
#include "QmParticle.h"
#include <iostream>


Quantum::QmAABB::QmAABB(QmBody* b)
{   
    QmParticle* p = (QmParticle*)b;
    min = p->getPos(0) - p->getRadius();
    max = p->getPos(0) + p->getRadius();
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