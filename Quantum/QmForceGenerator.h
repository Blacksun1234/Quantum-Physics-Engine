#pragma once
#include "QmParticle.h"

namespace Quantum {

	class QmParticle;

	class QmForceGenerator
	{
	public:
		virtual void update(QmParticle* p) {};
	};
}


