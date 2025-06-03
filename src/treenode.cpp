#include "treenode.h"
#include <vector>


// Serialize the object to a string representation
std::vector<char> TreeNode::serialize() const {
    std::vector<char> data;
    // Serialize the basic properties of the TreeNode
    Storable::appendData(data, Storable::serializeInt(id));
    Storable::appendData(data, Storable::serializeInt(level));
    Storable::appendData(data, Storable::serializeInt(parentID));

    // Serialize the children IDs
    int maxChildren = sizeof(childrenIDs) / sizeof(childrenIDs[0]);
    for (int i = 0; i < maxChildren; ++i) {
        Storable::appendData(data, Storable::serializeInt(childrenIDs[i]));
    }
    // Serialize the bounding box (rectangle)
    Storable::appendData(data, rectangle.serialize());
    return data;
}

// Deserialize the object from a string representation
static TreeNode deserialize(const std::vector<char>& data, int maxChildren, int dimensions) {
    if (data.size() < 3 + (maxChildren + 2 * dimensions) * sizeof(int)) {
        throw std::invalid_argument("Data size is too small for TreeNode deserialization.");
    }
    
    int id = Storable::deserializeInt(data, 0);
    int level = Storable::deserializeInt(data, sizeof(int));
    int parentID = Storable::deserializeInt(data, 2 * sizeof(int));

    int* childrenIDs = new int[maxChildren];
    int numChildren = 0; // Initialize number of children to 0
    for (int i = 0; i < maxChildren; ++i) {
        childrenIDs[i] = Storable::deserializeInt(data, 3 * sizeof(int) + i * sizeof(int));
        if(childrenIDs[i] != -1) {
            numChildren++;
        }
    }
    std::vector<char> regionData(data.begin() + (3 + maxChildren) * sizeof(int), data.end());
    Region rectangle = Region::deserialize(regionData, dimensions);

    return TreeNode(id, maxChildren, level, parentID, rectangle, childrenIDs);
}