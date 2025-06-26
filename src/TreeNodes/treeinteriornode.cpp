#include "treeinteriornode.h"

TreeInteriorNode ::TreeInteriorNode (int id, int maxChildren, int level, int parentID, const Region& boundingBox, int* childrenIDs, Region* childrenBoundingBoxes):
    TreeNode(id, level, parentID, boundingBox) // Call the base class constructor
{
    if(childrenIDs == nullptr) {
        throw std::invalid_argument("childrenIDs cannot be null when initializing TreeInteriorNode.");
    }
    if(childrenBoundingBoxes == nullptr) {
        throw std::invalid_argument("childrenBoundingBoxes cannot be null when initializing TreeInteriorNode.");
    }

    this->maxChildren = maxChildren;
    this->childrenIDs = new int[maxChildren];
    this->numChildren = 0;

    for (int i = 0; i < maxChildren; ++i) {
        this->childrenIDs[i] = childrenIDs[i];
        if (childrenIDs[i] != -1) {
            this->numChildren++; // Count the number of valid children
        }
    }

    this->childrenBoundingBoxes = new Region[maxChildren]; // Allocate memory for bounding boxes

    for (int i = 0; i < maxChildren; ++i) {
        this->childrenBoundingBoxes[i] = childrenBoundingBoxes[i];
    }
}

TreeInteriorNode ::~TreeInteriorNode () {
    delete[] childrenIDs; // Free the memory allocated for child IDs
    delete[] childrenBoundingBoxes; // Free the memory allocated for bounding boxes
}

void TreeInteriorNode::updateBoundingBox(){
    if (numChildren == 0) {
        boundingBox = Region(); // Reset to an empty region if no children
        return;
    }

    // create a vector of pointers to the children's bounding boxes
    std::vector<AbstractBoundedClass*> boundingBoxes;
    for (int i = 0; i < numChildren; ++i) {
        boundingBoxes.push_back(childrenBoundingBoxes + i);
    }
    boundingBox = Region::boundingBox(boundingBoxes); // Compute the bounding box of all children
}

void TreeInteriorNode::addChild(int childID, const Region& childBoundingBox) {
    if (numChildren >= maxChildren) {
        throw std::overflow_error("Node " + std::to_string(id) + " has reached its maximum number of children.");
    }

    // Add the new child
    childrenIDs[numChildren] = childID;
    childrenBoundingBoxes[numChildren] = childBoundingBox;
    numChildren++;

    // Update the bounding box of this node
    updateBoundingBox();
}

void TreeInteriorNode::addChildren(const std::vector<int>& childrenIDs, const std::vector<Region>& childrenBoundingBoxes) {
    if (numChildren + childrenIDs.size() > maxChildren) {
        throw std::overflow_error("Node " + std::to_string(id) + " cannot accommodate all new children.");
    }
    if (childrenIDs.size() != childrenBoundingBoxes.size()) {
        throw std::invalid_argument("childrenIDs and childrenBoundingBoxes must have the same size.");
    }

    for (size_t i = 0; i < childrenIDs.size(); ++i) {
        this->childrenIDs[numChildren] = childrenIDs[i];
        this->childrenBoundingBoxes[numChildren] = childrenBoundingBoxes[i];
        numChildren++;
    }

    // Update the bounding box of this node
    updateBoundingBox();
}

// Returns 0 for for success, -1 for failure
int TreeInteriorNode::removeChild(int childID) {
    for (int i = 0; i < numChildren; ++i) {
        if (childrenIDs[i] == childID) {
            // Shift remaining children down
            for (int j = i; j < numChildren - 1; ++j) {
                childrenIDs[j] = childrenIDs[j + 1];
                childrenBoundingBoxes[j] = childrenBoundingBoxes[j + 1];
            }
            numChildren--;
            childrenIDs[numChildren] = -1; // Mark the last child as invalid
            updateBoundingBox(); // Update the bounding box after removal
            return 0;
        }
    }
    return -1;
}

/*
===================================================
================== Storage stuff ==================
===================================================
*/

std::vector<char> TreeInteriorNode::serialize() const {
    std::vector<char> data = TreeNode::serialize(); // Serialize the base class first
    // preallocate space for the serialized data
    // #TODO: check if this is gives a performance boost
    // data.reserve(data.size() + getSerializedSize(maxChildren, boundingBox.getStart().size())); 

    // Serialize maxChildren
    Storable::appendData(data, Storable::serializeInt(maxChildren));

    // Serialize childrenIDs
    for (int i = 0; i < maxChildren; ++i) {
        Storable::appendData(data, Storable::serializeInt(childrenIDs[i]));
    }

    // Serialize childrenBoundingBoxes
    for (int i = 0; i < maxChildren; ++i) {
        Storable::appendData(data, childrenBoundingBoxes[i].serialize());
    }

    return data;
}

TreeInteriorNode TreeInteriorNode::deserialize(const std::vector<char>& data, int maxChildren, int dimensions) {
    // Deserialize the base class first
    TreeNode baseNode = TreeNode::deserialize(data, dimensions);

    // Deserialize maxChildren
    size_t offset = TreeNode::getSerializedSize(dimensions);
    int maxChildrenDeserialized = Storable::deserializeInt(data, offset);
    offset += sizeof(int);

    // Deserialize childrenIDs
    int* childrenIDs = new int[maxChildrenDeserialized];
    for (int i = 0; i < maxChildrenDeserialized; ++i) {
        childrenIDs[i] = Storable::deserializeInt(data, offset);
        offset += sizeof(int);
    }

    // Deserialize childrenBoundingBoxes
    Region* childrenBoundingBoxes = new Region[maxChildrenDeserialized];
    for (int i = 0; i < maxChildrenDeserialized; ++i) {
        std::vector<char> regionData(data.begin() + offset, data.end());
        childrenBoundingBoxes[i] = Region::deserialize(regionData, dimensions);
        offset += Region::getSerializedSize(dimensions);
    }

    return TreeInteriorNode(baseNode.getID(), maxChildrenDeserialized, baseNode.getLevel(), baseNode.getParentID(), baseNode.getBoundingBox(), childrenIDs, childrenBoundingBoxes);
}

int TreeInteriorNode::getSerializedSize(int maxChildren, int dimensions) {
    return TreeNode::getSerializedSize(dimensions) + // Size of the base class
           sizeof(int) + // Size of maxChildren
           maxChildren * (sizeof(int) + Region::getSerializedSize(dimensions)); // Size of childrenIDs and childrenBoundingBoxes
}