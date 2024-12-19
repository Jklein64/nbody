#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>

#include "body.hpp"
#include "node.hpp"

class Quadtree {
   public:
    // Empty Constructor
    Quadtree();
    // Constructor with position, size and time step
    Quadtree(double x, double y, double max, double min, double precision);
    // Destructor
    ~Quadtree();

    // Clean the Quadree
    void cleanQuadtree();

    // Find in which quadrant of a node the body should go. Then it inserts it.
    void findQuadrant(Body &body, Node &node);

    // Insert the body into a node
    void insertBody(Body &body, Node &node);

    // Create the leaves with the current node as the parent
    void createLeaves(Node &node);

    // Update the size of the root
    void updateRootSize(double w, double h);

    // Calculate all the accelerations between a body and a node and all of his children
    void calculateAllAccelerations(Body &body, Node &node);

    // Calculate all the accelerations starting from a node
    void calculateAllAccelerationsFromNode(Node &node);

    // Collect all the bodies in the tree (After updating their positions & velocities)
    void collect(std::vector<double> &data, Node &node);

    // Check if the body is lost in the space
    bool checkIfBodyIsLost(Body &body);

    // Variables
    double dt;                     // Time step
    double theta;                  // Precision parameter
    double minSize;                // Minimum size of a node
    Node root;                     // Root of the Quadtree
    std::vector<int> idCollision;  // Ids of the bodies that collided
};

#endif /* QUADTREE_H */