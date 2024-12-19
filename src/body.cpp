#include "body.hpp"

#include <math.h>

#include <iostream>

using namespace std;

// Empty Constructor
Body::Body(){};

// Contructor with mass, positions and velocities
Body::Body(double m, double x, double y)
    : mass(m), xPos(x), yPos(y), xAcc(0.0), yAcc(0.0), id(0){};

// Contructor with mass, positions, velocities and id
Body::Body(double m, double x, double y, int idBody)
    : mass(m), xPos(x), yPos(y), xAcc(0.0), yAcc(0.0), id(idBody){};

// Destructor
Body::~Body(){};

// Distance between this body and the one in the parameter
double Body::distance(Body &body) {
    return sqrt(pow(body.xPos - this->xPos, 2.0) + pow(body.yPos - this->yPos, 2.0));
}

// Function that will apply "its" forces to the body in parameter
void Body::applyForcesOnBody(Body &body) {
    if (body.id != this->id) {  // Avoid the same body

        double dist = this->distance(body);

        double cste = this->mass / pow(dist, 3.0);

        body.xAcc += G * (this->xPos - body.xPos) * cste;
        body.yAcc += G * (this->yPos - body.yPos) * cste;
    }
}

// Collide the body in the parameter into this body and return the id that will disappear
int Body::collide(Body &body) {
    int idx;
    // Update the position (We use a weighted average on the mass)
    this->xPos =
        (this->mass * this->xPos + body.mass * body.xPos) / (this->mass + body.mass);
    this->yPos =
        (this->mass * this->yPos + body.mass * body.yPos) / (this->mass + body.mass);
    // We don't update the acceleration since the bodies are collided during their
    // insertion into the tree

    // Update the Index if the mass of the body in parameter is bigger than the
    // mass of this body. Otherwise, we keep the index
    if (this->mass < body.mass) {
        idx = this->id;
        this->id = body.id;
    } else {
        idx = body.id;
    }

    // Sum the mass
    this->mass += body.mass;

    return idx;
}