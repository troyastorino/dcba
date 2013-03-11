#include "clothSystem.h"
#include <iostream>
#include <cstdlib>
#include <math.h>

using namespace std;

//TODO: Initialize here
ClothSystem::ClothSystem(int rows, int columns)
{
    int i, j;
    // calculate constants that will be used to make springs.
    const float root2 = sqrt(2);
    const float UNIT_LEN = 0.25f;

    const float STRUCT_SPRING_LEN = UNIT_LEN;
    const float STRUCT_SPRING_K = 20.0f;

    const float SHEAR_SPRING_K = 20.0f;
    const float SHEAR_SPRING_LEN = root2 * UNIT_LEN;

    const float FLEX_SPRING_K = 20.0f;
    const float FLEX_SPRING_LEN = 2.0f * UNIT_LEN;

    const float DRAG_K = 0.8f;
    const float GRAVITY = -0.2f;

    m_rows = rows;
    m_columns = columns;
	m_numParticles = m_rows*m_columns;

    vector<Vector3f> startState(2*m_numParticles);

    forces = new ForceStore();

    // fix the top corner particles
    forces->addZero(indexOf(0, 0, rows, columns));
    forces->addZero(indexOf(0, columns-1, rows, columns));

	for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++) {
            int thisIndex = indexOf(i, j, rows, columns);
            int right = indexOf(i, j+1, rows, columns);
            int bottom = indexOf(i+1, j, rows, columns);
            int topRight = indexOf(i-1, j+1, rows, columns);
            int bottomRight = indexOf(i+1, j+1, rows, columns);
            int secondRight = indexOf(i, j+2, rows, columns);
            int secondBottom = indexOf(i+2, j, rows, columns);

            //give it mass
            masses.push_back(1.0f);
            // damp it
            forces->addDrag(thisIndex, DRAG_K);
            // gravity acts on it
            forces->addAccel(thisIndex, Vector3f(0.0f, GRAVITY, 0.0f));

            // structural string to right neighbor
            if (right != -1) {
                forces->addSpring(thisIndex, right, STRUCT_SPRING_LEN, STRUCT_SPRING_K);
            }

            // structural string to bottom neighbor
            if (bottom != -1) {
                forces->addSpring(thisIndex, bottom, STRUCT_SPRING_LEN, STRUCT_SPRING_K);
            }

            // shear spring to top right neighbor
            if (topRight != -1) {
                forces->addSpring(thisIndex, topRight, SHEAR_SPRING_LEN, SHEAR_SPRING_K);
            }

            // shear spring to bottom right neighbor
            if (bottomRight != -1) {
                forces->addSpring(thisIndex, bottomRight, SHEAR_SPRING_LEN, SHEAR_SPRING_K);
            }

            // flex spring to second right neighbor
            if (secondRight != -1) {
                forces->addSpring(thisIndex, secondRight, FLEX_SPRING_LEN, FLEX_SPRING_K);
            }

            // flex spring to second bottom neighbor
            if (secondBottom != -1) {
                forces->addSpring(thisIndex, secondBottom, FLEX_SPRING_LEN, FLEX_SPRING_K);
            }

            // set the initial position and velocity of this particle
            startState[p(thisIndex)] = Vector3f(-2+UNIT_LEN*j, 2-UNIT_LEN*i, 0);
            startState[v(thisIndex)] = Vector3f(0, 0, 0);
        }
	}

    setState(startState);
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f = forces->calculate(state, masses);

	return f;
}

void ClothSystem::addMotion()
{
    int i, j;
    vector<Vector3f> state = getState();
    vector<Vector3f> newState(state.size());
    for(i = 0; i < m_rows; i++) {
        for(j = 0; j < m_columns; j++) {
            int thisIndex = indexOf(i, j, m_rows, m_columns);
            newState[p(thisIndex)] = state[p(thisIndex)];

            // add to each particles z velocity randomly, unless that particle is one of our fixed corners
            if (!(i == 0 && j == 0) && !(i == 0 && j == m_columns-1)) {

                Vector3f randVel = ((rand() % 90) / 30) * Vector3f(0, 0, 1);
                newState[v(thisIndex)] = state[v(thisIndex)] + randVel;
            }
        }
    }

    setState(newState);
}

///TODO: render the system (ie draw the particles)
void ClothSystem::draw()
{
    // draw each particle
    vector<Vector3f> mainState = getState();
	for (int i = 0; i < m_numParticles; i++) {
		Vector3f pos = mainState[p(i)];
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
    // draw each spring
    forces->drawSprings(mainState);
}

// returns the index of a particle at (i, j) in a mesh, where i is the row, j is the column
int indexOf(int i, int j, int rows, int columns) {
    if (i >= rows || j >= columns || i < 0 || j < 0) {
        return -1;
    }
    else {
        return i*columns + j;
    }
}
