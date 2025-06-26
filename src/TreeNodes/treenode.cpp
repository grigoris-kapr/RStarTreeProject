#include "treenode.h"
#include <vector>

TreeNode ::TreeNode (int id, int level, int parentID, const Region& boundingBox) {
    this->id = id;
    this->level = level;
    this->parentID = parentID;
    this->numChildren = 0; // Init here to be used in interior and leaf nodes

    this->boundingBox = boundingBox; // Set the bounding box
}

TreeNode ::~TreeNode () {
}

// Serialize the object to a string representation
std::vector<char> TreeNode::serialize() const {
    std::vector<char> data;
    // Serialize the basic properties of the TreeNode
    Storable::appendData(data, Storable::serializeInt(id));
    Storable::appendData(data, Storable::serializeInt(level));
    Storable::appendData(data, Storable::serializeInt(parentID));

    // Serialize the bounding box 
    Storable::appendData(data, boundingBox.serialize());
    return data;
}

// Deserialize the object from a string representation
TreeNode TreeNode::deserialize(const std::vector<char>& data, int dimensions) {
    if (data.size() < TreeNode::getSerializedSize(dimensions)) {
        throw std::invalid_argument("Data size is too small for TreeNode deserialization.");
    }
    
    int id = Storable::deserializeInt(data, 0);
    int level = Storable::deserializeInt(data, sizeof(int));
    int parentID = Storable::deserializeInt(data, 2 * sizeof(int));

    
    std::vector<char> regionData(data.begin() + 3 * sizeof(int), data.end());
    Region boundingBox = Region::deserialize(regionData, dimensions);

    return TreeNode(id, level, parentID, boundingBox);
}

// Get the size of the serialized object
int TreeNode::getSerializedSize(int dimensions) {
    return 3 * sizeof(int) + Region::getSerializedSize(dimensions);
}