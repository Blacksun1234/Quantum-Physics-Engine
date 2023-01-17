#ifndef QMWORLD_H
#define QMWORLD_H

#include <list>
#include <vector>

namespace Quantum {

	class QmBody;
	class QmForceRegistery;

	class QmWorld {
	public:
		QmWorld();
		~QmWorld();
		void simulate(float);
		void addBody(QmBody*);
		std::vector<QmBody*> getBodies();

		void clear();
	private:
		float time;
		std::vector<QmBody*> bodies;
		void integrate(float);
		void applyGravity();
		void updateForces();
		void addForceRegistery(QmForceRegistery* f);
	};

}

#endif