#ifndef QMWORLD_H
#define QMWORLD_H


#include <glm/glm.hpp>

#include <list>
#include <vector>

#include "QmContact.h"
#include "QmAABB.h"

namespace Quantum {

	class QmBody;
	class QmForceRegistery;

	class QmWorld {
	public:
		QmWorld();
		~QmWorld();
		void simulate(float);
		float tickRK4(float);
		float tick(float);
		void interpolate(float);
		void addBody(QmBody*);
		std::vector<QmBody*> getBodies();
		void SetGravity(bool isGravityActive);
		void SetCollision(bool isCollisionActive);
		void SetBroadPhase(bool isBroadPhaseActive);
		void SetDelta();
		bool GetDelta();
		void clear();
		void clearScene();
		void addForceRegistery(QmForceRegistery* f);
		int getNumericalIntegrator();
		bool getGravityIsActive();
		bool getCollisionIsActive();
		bool getBroadPhaseIsActive();
		void changeNumericalIntegrator();
		std::vector<QmForceRegistery*> QmWorld::getForcesRegistery();
		std::vector<QmBody*> bodies;
		std::vector<QmForceRegistery*> forcesRegistries;

		std::vector<QmContact> broadphase();
		std::vector<QmContact> narrowphase(std::vector<QmContact> c);
		bool intersect(QmAABB a, QmAABB b);
		void resolve(std::vector<QmContact> c);
		

	private:
		bool _delta;
		float _time;
		float _ticktime;
		glm::vec3 gravity;
		bool _isGravityActive;
		bool _isCollisionActive;
		bool _isBroadPhaseActive;
		void integrate(float, int);
		void integrate_RK4(float);
		void applyGravity(int);
		void updateForces(int);
		int numericalIntegrator = 0;
	};
}
#endif