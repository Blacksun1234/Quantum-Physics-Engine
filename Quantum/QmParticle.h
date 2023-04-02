#ifndef QMPARTICLE_H
#define QMPARTICLE_H

#include <glm/glm.hpp>
#include "QmBody.h"
#include "QmAABB.h"

namespace Quantum {
	class QmUpdater;
	class QmAABB;

	class QmParticle : public QmBody {
	public:
		QmParticle();
		QmParticle(glm::vec3, glm::vec3, glm::vec3, float, float, float);
		~QmParticle();
		void integrateExplicit(float, int);
		void integrateSemiExplicit(float, int);
		void integrateRK4(float, int);
		void integrate_RK4(float);

		glm::vec3 getAcc(int);
		glm::vec3 getVel(int);
		glm::vec3 getPos(int);
		float getRadius();
		float getCharge();
		float getMass();
		float getRestitution();

		void SetAcc(glm::vec3 acc, int);
		void SetPos(glm::vec3 pos, int);
		void SetVel(glm::vec3 vel, int);
		QmUpdater* QmParticle::GetUpdater();
		void setUpdater(QmUpdater* updater);
		void addForce(glm::vec3, int);
		void clearParticle();

		QmAABB getAABB();

	private:
		QmUpdater* updater;
		/*glm::vec3 _position;
		glm::vec3 _velocity;
		glm::vec3 _acceleration;
		glm::vec3 _forceAccumulator;*/

		glm::vec3 _position[4];
		glm::vec3 _velocity[4];
		glm::vec3 _acceleration[4];
		glm::vec3 _forceAccumulator[4];
		float invMass;
		float _mass;

		float damping;
		float _charge;

		float _radius;
		float _restitution;
	};
}
#endif