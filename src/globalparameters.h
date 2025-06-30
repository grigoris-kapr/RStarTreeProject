/*
 * GlobalParameters.h
 * This struct stores the parameters needed for various classes in the project.
 * It helps avoid storing them per each object and unifies the Storable interface.
*/

struct GlobalParameters {
    int maxChildren; // Maximum number of children per node
    int dimensions; // Dimensionality of the points
};
