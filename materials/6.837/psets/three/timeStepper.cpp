#include "timeStepper.hpp"
#include <iostream>

///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    unsigned i;

    vector<Vector3f> systemState = particleSystem->getState();

    // take a single derivative
    vector<Vector3f> deriv = particleSystem->evalF(systemState);

    // create the final state by incrementing the last state
    vector<Vector3f> finalState(systemState.size());

    for (i = 0; i < finalState.size(); i++) {
        finalState[i] = systemState[i] + (stepSize * deriv[i]);
    }

    // set the final state as the current particle state
    particleSystem->setState(finalState);
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    unsigned i, j;

    // get the current state
    vector<Vector3f> systemState = particleSystem->getState();

    // take a single derivative
    vector<Vector3f> deriv = particleSystem->evalF(systemState);

    // create the new state by incrementing the last state
    vector<Vector3f> newState(systemState.size());

    for (i = 0; i < newState.size(); i++) {
        newState[i] = systemState[i] + (stepSize * deriv[i]);
    }

    // take a derivative at the second state
    vector<Vector3f> newDeriv = particleSystem->evalF(newState);

    // construct the final state based on this information
    vector<Vector3f> finalState(systemState.size());

    for (j = 0; j < finalState.size(); j++) {
        finalState[j] = systemState[j] + (stepSize * (deriv[j] + newDeriv[j]) / 2);
    }

    // set the final state as the current particle state
    particleSystem->setState(finalState);
}
