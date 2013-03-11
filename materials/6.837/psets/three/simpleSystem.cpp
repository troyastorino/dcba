#include "simpleSystem.h"
#include <iostream>

using namespace std;

SimpleSystem::SimpleSystem()
{
    m_numParticles = 1;
    vector<Vector3f> startState(1);
    startState[0] = Vector3f(1, 1, 1);
    setState(startState);
}

// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
    unsigned i;
	vector<Vector3f> f(state.size());

    for (i = 0; i < state.size(); i++) {
        f[i] = Vector3f(-1 * state[i][1], state[i][0], 0);
    }

	return f;
}

void SimpleSystem::addMotion()
{
    // do nothing
    return;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
    Vector3f pos = getState()[0];
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    glutSolidSphere(0.075f,10.0f,10.0f);
    glPopMatrix();
}
