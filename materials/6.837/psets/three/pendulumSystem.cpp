#include "pendulumSystem.h"
#include "forceStore.h"

using namespace std;

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = m_numParticles;

    vector<Vector3f> startState(2*numParticles);

    forces = new ForceStore();

    // lock the first point
    forces->addZero(0);

	for (int i = 0; i < m_numParticles; i++) {

        //give it mass
        masses.push_back(1.0f);

        if (i != 0) {
            // connect it to the previous with a spring
            forces->addSpring(i-1, i, 0.25f, 2.0f);
            // gravity acts on it
            forces->addAccel(i, Vector3f(0.0f, -0.5f, 0.0f));
            // add a damping force
            forces->addDrag(i, 0.2f);
        }

		// for this system, we care about the position and the velocity
        startState[2*i] = Vector3f(0, -0.25*i, 0);

        if (i == m_numParticles - 1) {
            startState[2*i + 1] = Vector3f(3, 0, 0);
        }
        else {
            startState[2*i + 1] = Vector3f(0, 0, 0);
        }
	}

    setState(startState);
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f = forces->calculate(state, masses);

	return f;
}

void PendulumSystem::addMotion()
{
    // do nothing
    return;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
    vector<Vector3f> mainState = getState();
	for (int i = 0; i < m_numParticles; i++) {
		Vector3f pos = mainState[2*i];
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}

    // draw each spring
    forces->drawSprings(mainState);
}
