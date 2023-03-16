#pragma once
#ifndef QMBODY_H
#define QMBODY_H

#include <glm/glm.hpp>
#include "QmUpdater.h"

namespace Quantum {

	const int TYPE_PARTICLE = 0;
	
	class QmBody {
	public:
		virtual void integrateExplicit(float, int) {};
		virtual void integrateSemiExplicit(float, int) {};
		virtual void integrateRK4(float, int) {};
		virtual void integrate_RK4(float) {};
		virtual void clearParticle() {};
		int getType() const { return type; }
		virtual void SetAcc(glm::vec3 acc, int) {};

	protected:
		int type;
	private:
	};
}

#endif