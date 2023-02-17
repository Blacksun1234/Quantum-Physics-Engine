#ifndef QMWORLD_H
#define QMWORLD_H


#include <glm/glm.hpp>

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
		float tick(float);
		void interpolate(float);
		void addBody(QmBody*);
		std::vector<QmBody*> getBodies();
		void SetGravity(bool isGravityAcitve);
		void SetDelta(bool);
		bool GetDelta();
		void clear();
		void clearScene();
		void addForceRegistery(QmForceRegistery* f);
		std::vector<QmBody*> bodies;
		std::vector<QmForceRegistery*> forcesRegistries;

	private:
		bool _delta;
		float _time;
		float _ticktime;
		glm::vec3 gravity;
		bool _isGravityActive;
		void integrate(float);
		void applyGravity();
		void updateForces();
		
	};

}

#endif