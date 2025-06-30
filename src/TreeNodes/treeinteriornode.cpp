#include "treeinteriornode.h"

TreeInteriorNode::TreeInteriorNode (GlobalParameters* config, int id, int level, int parentID, const Region& boundingBox, std::vector<int> childrenIDs, Region* childrenBoundingBoxes):
    TreeNode(id, level, parentID, boundingBox) // Call the base class constructor
{
    if(childrenIDs.size() != config->maxChildren) {
        throw std::invalid_argument("childrenIDs size must match maxChildren.");
    }
    if(childrenBoundingBoxes == nullptr) {
        throw std::invalid_argument("childrenBoundingBoxes cannot be null when initializing TreeInteriorNode.");
    }

    this->childrenIDs = childrenIDs;
    this->numChildren = 0;

    for(int i = 0; i < config->maxChildren; ++i) {
        if(childrenIDs[i] >=0) { // count non-empty children
            this->numChildren++;
        }
    }

    this->childrenBoundingBoxes = new Region[config->maxChildren]; // Allocate memory for bounding boxes

    for (int i = 0; i < config->maxChildren; ++i) {
        this->childrenBoundingBoxes[i] = childrenBoundingBoxes[i];
    }
}

TreeInteriorNode::~TreeInteriorNode () {
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

void TreeInteriorNode::addChild(GlobalParameters* config, int childID, const Region& childBoundingBox) {
    if (numChildren >= config->maxChildren) {
        throw std::overflow_error("Node " + std::to_string(id) + " has reached its maximum number of children.");
    }

    // Add the new child
    childrenIDs[numChildren] = childID;
    childrenBoundingBoxes[numChildren] = childBoundingBox;
    numChildren++;

    // Update the bounding box of this node
    updateBoundingBox();
}

void TreeInteriorNode::addChildren(GlobalParameters* config, const std::vector<int>& childrenIDs, const std::vector<Region>& childrenBoundingBoxes) {
    if (numChildren + childrenIDs.size() > config->maxChildren) {
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
// TODO: Make this not slow
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

// Return all IDs that overlap the query
// Supports both point and region queries
std::vector<int> TreeInteriorNode::rangeQuery(const AbstractBoundedClass& query) const {
    std::vector<int> results;

    // Check if the query overlaps with this node's bounding box
    // TODO: performance check this
    if (!boundingBox.overlaps(query)) {
        return results; // No overlap, return empty result
    }

    // Iterate through children and add id if overlap
    for (int i = 0; i < numChildren; ++i) {
        if (childrenBoundingBoxes[i].overlaps(query)) {
            results.push_back(childrenIDs[i]); 
        }
    }

    return results;
}

/*
===================================================
================== Storage stuff ==================
===================================================
*/

// Serialize the node to a string representation
// Doesn't store numChildren, as it can be derived from the data
// and maxChildren as it's stored in data block 0
std::vector<char> TreeInteriorNode::serialize(GlobalParameters* config) const {
    std::vector<char> data = TreeNode::serialize(config); // Serialize the base class first
    // preallocate space for the serialized data
    // #TODO: check if this is gives a performance boost
    // data.reserve(data.size() + getSerializedSize(maxChildren, boundingBox.getStart().size())); 

    // Serialize childrenIDs
    Storable::appendData(data, Storable::serializeInts(childrenIDs));

    // Serialize childrenBoundingBoxes
    for (int i = 0; i < numChildren; ++i) {
        Storable::appendData(data, childrenBoundingBoxes[i].serialize(config));
    }

    // Pad out for empty children
    Storable::appendData(data, std::vector<char>((config->maxChildren - numChildren) * Region::getSerializedSize(config), 0));

    return data;
}

TreeInteriorNode TreeInteriorNode::deserialize(GlobalParameters* config, const std::vector<char>& data) {
    if (data.size() < TreeInteriorNode::getSerializedSize(config)) {
        throw std::invalid_argument("Data size is too small for TreeInteriorNode deserialization.");
    }
    // Deserialize the base class first
    TreeNode baseNode = TreeNode::deserialize(config, data);
    size_t offset = TreeNode::getSerializedSize(config);

    // Deserialize childrenIDs
    std::vector<int> childrenIDs = Storable::deserializeInts(data, offset, config->maxChildren);
    offset += config->maxChildren * sizeof(int); // Move the offset past the childrenIDs
    // Count the children
    int numChildren = 0;
    for(int i = 0; i < config->maxChildren; ++i) {
        if(childrenIDs[i] >=0) { // count non-empty children
            numChildren++;
        }
    }

    // Deserialize childrenBoundingBoxes
    Region* childrenBoundingBoxes = new Region[config->maxChildren];
    for (int i = 0; i < numChildren; ++i) {
        std::vector<char>::const_iterator regionDataStart = data.begin() + offset;
        std::vector<char>::const_iterator regionDataEnd = regionDataStart + Region::getSerializedSize(config);
        std::vector<char> regionData(regionDataStart, regionDataEnd);

        childrenBoundingBoxes[i] = Region::deserialize(config, regionData);
        offset += Region::getSerializedSize(config);
    }
    // Don't bother with the rest, set to empty regions
    for (int i = numChildren; i < config->maxChildren; ++i) {
        childrenBoundingBoxes[i] = Region(); // Default constructor creates an empty region
    }

    return TreeInteriorNode(config, baseNode.getID(), baseNode.getLevel(), baseNode.getParentID(), baseNode.getBoundingBox(), childrenIDs, childrenBoundingBoxes);
}

int TreeInteriorNode::getSerializedSize(GlobalParameters* config) {
    return TreeNode::getSerializedSize(config) + // Size of the base class
           config->maxChildren * (sizeof(int) + Region::getSerializedSize(config)); // Size of childrenIDs and childrenBoundingBoxes
}