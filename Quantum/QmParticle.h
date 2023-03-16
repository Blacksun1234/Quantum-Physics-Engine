#ifndef QMPARTICLE_H
#define QMPARTICLE_H

#include <glm/glm.hpp>
#include "QmBody.h"


namespace Quantum {
	class QmUpdater;

	class QmParticle : public QmBody {
	public:
		QmParticle();
		QmParticle(glm::vec3, glm::vec3, glm::vec3, float, float);
		~QmParticle();
		void integrateExplicit(float, int);
		void integrateSemiExplicit(float, int);
		void integrateRK4(float, int);
		void integrate_RK4(float);

		glm::vec3 getAcc(int);
		glm::vec3 getVel(int);
		glm::vec3 getPos(int);
		float getCharge();

		void SetAcc(glm::vec3 acc, int);
		QmUpdater* QmParticle::GetUpdater();
		void setUpdater(QmUpdater* updater);
		void addForce(glm::vec3, int);
		void clearParticle();

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

		float damping;
		float _charge;
	};


	
}

#endif