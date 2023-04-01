#pragma once
#ifndef QMBODY_H
#define QMBODY_H

#include <glm/glm.hpp>
#include "QmUpdater.h"
#include "QmAABB.h"

using namespace Quantum;

namespace Quantum {

	const int TYPE_PARTICLE = 0;
	class QmAABB;
	
	class QmBody {
	public:
		virtual void integrateExplicit(float, int) {};
		virtual void integrateSemiExplicit(float, int) {};
		virtual void integrateRK4(float, int) {};
		virtual void integrate_RK4(float) {};
		virtual void clearParticle() {};
		int getType() const { return type; }
		virtual void SetAcc(glm::vec3 acc, int) {};
		virtual QmAABB getAABB() = 0;
	protected:
		int type;
	private:
	};
}

#endif