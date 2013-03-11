#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include <vecmath.h>
#include <vector>
#include <GL/glut.h>

#include "particleSystem.h"
#include "forceStore.h"

class PendulumSystem: public ParticleSystem
{
public:
	PendulumSystem(int numParticles);

	vector<Vector3f> evalF(vector<Vector3f> state);

    void addMotion();

	void draw();

protected:
    ForceStore *forces;
    vector<float> masses;
};

#endif
