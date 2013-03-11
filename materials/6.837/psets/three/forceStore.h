#ifndef FORCESTORE_H
#define FORCESTORE_H

#include <vector>
#include <vecmath.h>
#include <GL/glut.h>
#include <string>

using namespace std;

// the ForceStore object can be used to easily add forces to a system
// and calculate the resulting force on the system. This could probably
// be a part of ParticleSystem, but I liked it like this.
class ForceStore
{
    public:
        ForceStore();

        // add a new drag force to the system
        void addDrag(int particle, float k);

        // add a new constant acceleration to the system
        void addAccel(int particle, Vector3f accel);

        // add a new spring-like force to the system
        void addSpring(int p1, int p2, float r, float k);

        // add a particle that should always have a net force of 0 on it
        void addZero(int particle);

        // use all the information provided thus far through add* to calculate
        // the derivative of the system
        vector<Vector3f> calculate(vector<Vector3f> state, vector<float> masses);

        // draw the springs as lines
        void drawSprings(vector<Vector3f> state);

    protected:

        vector<Vector2f> drags;
        vector<Vector4f> accelerations;
        vector<Vector4f> springs;
        vector<int> zeros;
};

int p(int i);
int v(int i);

#endif
