#pragma once
#include <glm/glm.hpp>
#include "QmParticle.h"

namespace Quantum {
	class QmAABB
	{
	public:
		QmAABB(QmBody*);
		glm::vec3 getMin();
		glm::vec3 getMax();

	private :
		glm::vec3 min;
		glm::vec3 max;
	};
}