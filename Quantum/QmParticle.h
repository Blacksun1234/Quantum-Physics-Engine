#ifndef QMPARTICLE_H
#define QMPARTICLE_H

#include <glm/glm.hpp>
#include "QmBody.h"


namespace Quantum {
	class QmUpdater;

	class QmParticle : public QmBody {
	public:
		QmParticle();
		QmParticle(glm::vec3, glm::vec3, glm::vec3);
		~QmParticle();
		virtual void integrate(float);
		
		glm::vec3 getAcc();
		glm::vec3 getVel();
		glm::vec3 getPos();

		void setUpdater(QmUpdater* updater);


	private:
		QmUpdater* updater;
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;

		float damping;

	};

}

#endif