#ifndef NODE_H
#define NODE_H

#include "body.hpp"

class Node {
   public:
    // Empty Constructor
    Node();
    // Constructor with position and size
    Node(double x, double y, double w, double h);
    // Constructor with parent, position and size
    Node(Node* ptr, double x, double y, double w, double h);
    // Destructor
    ~Node();

    // Delete the nodes
    void deleteNodes();

    // Add the first body
    void addFirstBody(Body& body);

    // Update the node when another body is inserted
    void insertBodyAndUpdateNode(Body& body);

    // Distance between this node and the body in the parameter
    double distance(Body& body);

    // Function that will apply "its" forces to the body in parameter
    void applyForcesOnBody(Body& body);

    // Remove 1 body from the number of body recursively
    void removeBody();

    // Test if the size is big enough for printing
    bool testSize(double size);

    // Variables for the node
    double xCenter;  // X position of the center of the node
    double yCenter;  // Y position of the center of the node
    double width;    // Width of the node
    double height;   // Height of the node
    // Variables for the center of mass
    Body localBody;     // Local body
    double mass;        // Total mass inside the node
    double xPosCM;      // X position of the center of mass
    double yPosCM;      // Y position of the center of mass
    int nbrBodies;      // Number of bodies inside the node
    bool containsBody;  // Say if the Node contains a body
    // Variables for the Quadtree
    Node* parent;     // Parent node in the Quadtree
    Node* northEast;  // Node in the North-East direction
    Node* northWest;  // Node in the North-West direction
    Node* southEast;  // Node in the South-East direction
    Node* southWest;  // Node in the South-West direction
    bool isLeaf;      // Say if the tree is a leaf
    int depth;        // Depth of the node in the Quadtree
    bool root;        // Tells if the node is the root in the Quadtree
    // Variable specific for ParallelQuadtree
};

#endif /* NODE_H */