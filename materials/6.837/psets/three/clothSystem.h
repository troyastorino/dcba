#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vecmath.h>
#include <vector>
#include <GL/glut.h>

#include "particleSystem.h"
#include "forceStore.h"

class ClothSystem: public ParticleSystem
{
public:
	ClothSystem(int rows, int columns);

	vector<Vector3f> evalF(vector<Vector3f> state);

    void addMotion();

	void draw();

protected:
    ForceStore *forces;
    vector<float> masses;
    int m_rows;
    int m_columns;
};

// returns the index of a particle at (i, j) in a mesh, where i is the row, j is the column
int indexOf(int i, int j, int rows, int columns);

#endif
