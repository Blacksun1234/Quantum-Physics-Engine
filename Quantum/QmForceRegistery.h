
namespace Quantum {

	class QmParticle;
	class QmForceGenerator;

	class QmForceRegistery
	{
	public:
		QmForceRegistery(QmParticle* _p, QmForceGenerator* _fg);
		QmParticle* p;
		QmForceGenerator* fg;
	};
}



