#include "forceStore.h"
#include <iostream>

using namespace std;

ForceStore::ForceStore()
{

}

// add a new drag force to the system
void ForceStore::addDrag(int particle, float k) {
    drags.push_back(Vector2f(particle, k));
}

// add a new constant acceleration to the system
void ForceStore::addAccel(int particle, Vector3f accel) {
    accelerations.push_back(Vector4f(particle, accel));
}
// add a new spring-like force to the system
void ForceStore::addSpring(int p1, int p2, float r, float k) {
    springs.push_back(Vector4f(p1, p2, r, k));
}

// add a particle that should always have a net force of 0 on it
void ForceStore::addZero(int particle) {
    zeros.push_back(particle);
}

// use all the information provided thus far through add* to calculate
// the derivative of the system
vector<Vector3f> ForceStore::calculate(vector<Vector3f> state, vector<float> masses) {
    unsigned d, a, s, z, i;
    vector<Vector3f> f(state.size());

    // iterate over all particles, setting x deriv to v and v deriv to 0
    for (i = 0; i < state.size()/2; i++) {
        f[p(i)] = state[v(i)];
        f[v(i)] = Vector3f(0, 0, 0);
    }

    // iterate over drags
    for (d = 0; d < drags.size(); d++) {
        int particle = drags[d][0];
        Vector3f force = -1 * drags[d][1] * state[v(particle)];
        f[v(particle)] = f[v(particle)] + force;
    }


    // iterate over accelerations
    for (a = 0; a < accelerations.size(); a++) {
        int particle = accelerations[a][0];
        Vector3f force = masses[0] * accelerations[a].yzw();
        f[v(particle)] = f[v(particle)] + force;
    }

    for (s = 0; s < springs.size(); s++) {
        int p1 = springs[s][0];
        int p2 = springs[s][1];

        float r = springs[s][2];
        float k = springs[s][3];

        Vector3f posDiff = state[p(p1)] - state[p(p2)];
        float distance = posDiff.abs();
        Vector3f force1 = -1 * k * (distance - r) * posDiff.normalized();
        Vector3f force2 = -1 * force1;

        f[v(p1)] = f[v(p1)] + force1;
        f[v(p2)] = f[v(p2)] + force2;
    }

    for (z = 0; z < zeros.size(); z++) {
        int particle = zeros[z];
        f[v(particle)] = Vector3f(0.0, 0.0, 0.0);
    }


    return f;
}


// draw the springs as lines
void ForceStore::drawSprings(vector<Vector3f> state) {

    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glDisable(GL_LIGHTING);
    glLineWidth(1.0f);
    glPushMatrix();
    glBegin(GL_LINES);
    glColor4f(0.4f, 1.0f, 0.4f, 1.0f);

	for (int i = 0; i < springs.size(); i++) {
        int p1 = springs[i][0];
        int p2 = springs[i][1];

		Vector3f pos1 = state[p(p1)];
		Vector3f pos2 = state[p(p2)];

        glVertex3f( pos1[0], pos1[1], pos1[2] );
        glVertex3f( pos2[0], pos2[1], pos2[2] );
	}

    glEnd();

    glPopMatrix();

    glPopAttrib();

}

int p(int i) {
    return 2*i;
}
int v(int i) {
    return (2*i) + 1;
}
