#pragma once
#include <glm/glm.hpp>


namespace Quantum {
	class QmBody;

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