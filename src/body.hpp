#ifndef BODY_H
#define BODY_H

// #define G 6.674e-11

// Class defining body stored in nodes of the tree
class Body {
   public:
    // Empty Constructor
    Body();
    // Contructor with mass, positions and velocities
    Body(double m, double x, double y);
    // Contructor with mass, positions, velocities and id
    Body(double m, double x, double y, int idBody);
    // Destructor
    ~Body();

    // Distance between this body and the one in the parameter
    double distance(Body &body);

    // Function that will apply "its" forces to the body in parameter
    void applyForcesOnBody(Body &body);
    int collide(Body &body);

    // Variables
    double mass;  // Mass of the body
    double xPos;  // X Position of the body
    double yPos;  // Y Position of the body
    double xAcc;  // X Acceleration of the body
    double yAcc;  // Y Acceleration of the body
    int id;       // ID of the body (Useful when we try to distinguish two bodies)
    // Only used by the Barnes-Hut parallel
};

#endif /* BODY_H */