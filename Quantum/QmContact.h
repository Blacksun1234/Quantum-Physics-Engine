#pragma once
#include <glm/glm.hpp>
#include "QmBody.h"

namespace Quantum {
	class QmContact
	{
	public:
		QmContact(QmBody* b1, QmBody* b2);
		QmBody* getBody1();
		QmBody* getBody2();
		glm::vec3 getNormal();
		float getDepth();
		void setNormal(glm::vec3 normal);
		void setDepth(float depth);
	private:
		QmBody* _body1;
		QmBody* _body2;
		glm::vec3 _normal;
		float _depth;
	};
}

