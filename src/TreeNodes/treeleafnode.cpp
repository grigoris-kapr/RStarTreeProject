#include "treeleafnode.h"
#include <cstring>


// Constructor for TreeLeafNode
// Used when deserializing or pass empty vectors for initialization
// Supports vectors of the same size, up to maxChildren and pads with empty values if necessary
// Throws an error if the input vectors are not of the same size or if they exceed max
// Throws an error if one of the input vectors contains empty slots where at least one other doesn't
TreeLeafNode::TreeLeafNode(GlobalParameters* config, int id, int level, int parentID, const Region& boundingBox, const std::vector<Point>& points, const std::vector<int>& blockIDs, const std::vector<int>& recordIDs)
    : TreeNode(id, level, parentID, boundingBox)
{
    // All input vectors of the same size
    if (points.size() != blockIDs.size() || points.size() != recordIDs.size()) {
        throw std::invalid_argument("Points, block IDs and record IDs must have the same size.");
    }
    // if input vectors size is larger than maxChildren, throw an error
    if (points.size() > config->maxChildren) {
        throw std::overflow_error("Number of points exceeds maximum children allowed in the leaf node.");
    }

    // pass to self for editability
    this->blockIDs = blockIDs;
    this->recordIDs = recordIDs;
    this->points = points;

    // if input vectors size is smaller than maxChildren, set numChildren and pad with empty values
    if (this->points.size() < config->maxChildren) {
        numChildren = this->points.size();
        // -1 and Point() for empty slots
        this->blockIDs.resize(config->maxChildren, -1);
        this->recordIDs.resize(config->maxChildren, -1);
        this->points.resize(config->maxChildren, Point());
    }
    // if input vectors size is equal to maxChildren, either we have that many children or we have empty slots
    else {
        numChildren = 0;
        for (size_t i = 0; i < this->points.size(); ++i) {
            // if none indicate empty, count as a child
            if (this->blockIDs[i] != -1 && this->recordIDs[i] != -1 && this->points[i] != Point()) {
                numChildren++;
            }
            // if NOT ALL indicate empty, throw an error
            else {
                // After serialization, an empty point is at 0,0,...,0
                Point zeroPoint = Point(std::vector<double>(this->points[0].getCoordinates().size(), 0.0));
                if (!(this->blockIDs[i] == -1 && this->recordIDs[i] == -1 && (points[i] == Point() || points[i] == zeroPoint))) {
                    throw std::invalid_argument("Invalid point, block ID or record ID at index " + std::to_string(i) + ".\n"
                        "Got: " + this->printPointInfo(config, i) + ".\n");
                }
            }
        }
    }
}

std::string TreeLeafNode::printPointInfo(GlobalParameters* config, int i) const {
    if (i < 0 || i >= config->maxChildren) {
        throw std::out_of_range("Index out of range for point info.");
    }
    return "Point: " + points[i].toString(config) + ", Block ID: " + std::to_string(blockIDs[i]) + ", Record ID: " + std::to_string(recordIDs[i]);
}

int TreeLeafNode::findPointIndex(const Point& point) const {
    for (int i = 0; i < numChildren; ++i) {
        if (points[i] == point) {
            return i; // Return the index of the found point
        }
    }
    return -1; // Point not found
}

// Add a point to the leaf node
// Throws an error if the point already exists or if the maximum number of children is reached
void TreeLeafNode::addPoint(GlobalParameters* config, const Point& point, int blockID, int recordID) {
    if (numChildren >= config->maxChildren) {
        throw std::overflow_error("Node " + std::to_string(id) + " has reached its maximum number of children.");
    }
    // Check if a point already exists at that location
    int existingIndex = findPointIndex(point);
    if (existingIndex != -1) {
        throw std::invalid_argument("Point already exists in the leaf node:\n" + this->printPointInfo(config, existingIndex) + ".");
    }
    if (blockID < 0 || recordID < 0) {
        throw std::invalid_argument("Block ID and Record ID must be non-negative.");
    }

    // Add the point and its IDs
    points[numChildren] = point;
    blockIDs[numChildren] = blockID;
    recordIDs[numChildren] = recordID;
    numChildren++;
}

// Add multiple points to the leaf node
// Throws an error if the input vectors are not of the same size or if the maximum number of children is reached
// Does not check if they already exist
void TreeLeafNode::addPoints(GlobalParameters* config, const std::vector<Point>& points, const std::vector<int>& blockIDs, const std::vector<int>& recordIDs) {
    if (points.size() != blockIDs.size() || points.size() != recordIDs.size()) {
        throw std::invalid_argument("Points, block IDs and record IDs must have the same size.");
    }
    if (numChildren + points.size() > config->maxChildren) {
        throw std::overflow_error("Node " + std::to_string(id) + " has reached its maximum number of children.");
    }

    for(int i = 0; i < points.size(); ++i) {
        // Add the point and its IDs
        this->points[numChildren] = points[i];
        this->blockIDs[numChildren] = blockIDs[i];
        this->recordIDs[numChildren] = recordIDs[i];
        numChildren++;
    }
}

std::pair<int, int> TreeLeafNode::findPoint(const Point& point) const {
    for (int i = 0; i < numChildren; ++i) {
        if (points[i] == point) {
            return {blockIDs[i], recordIDs[i]};
        }
    }
    return {-1, -1}; // Point not found
}

std::vector<std::pair<int, int>> TreeLeafNode::rangeQuery(const AbstractBoundedClass& query) const {
    Region queryRegion = static_cast<const Region&>(query);
    std::vector<std::pair<int, int>> results;
    for (int i = 0; i < numChildren; ++i) {
        if (queryRegion.overlaps(points[i])) {
            results.emplace_back(blockIDs[i], recordIDs[i]);
        }
    }
    return results;
}

int TreeLeafNode::removePoint(int blockID, int recordID) {
    for (int i = 0; i < numChildren; ++i) {
        if (blockIDs[i] == blockID && recordIDs[i] == recordID) {
            // Shift the remaining points to fill the gap
            for (int j = i; j < numChildren - 1; ++j) {
                points[j] = points[j + 1];
                blockIDs[j] = blockIDs[j + 1];
                recordIDs[j] = recordIDs[j + 1];
            }
            numChildren--;
            return 0;
        }
    }
    return -1; // Point not found
}

int TreeLeafNode::removePoint(const Point& point) {
    int index = findPointIndex(point);
    if (index == -1) {
        return -1; // Point not found
    }
    // Shift the remaining points to fill the gap
    for (int j = index; j < numChildren - 1; ++j) {
        points[j] = points[j + 1];
        blockIDs[j] = blockIDs[j + 1];
        recordIDs[j] = recordIDs[j + 1];
    }
    numChildren--;
    return 0;
}

/*
===================================================
================== Storage stuff ==================
===================================================
*/

std::vector<char> TreeLeafNode::serialize(GlobalParameters* config) const {
    std::vector<char> data = TreeNode::serialize(config);
    
    // Serialize the points, blockIDs and recordIDs
    Storable::appendData(data, Storable::serializeInts(blockIDs));
    Storable::appendData(data, Storable::serializeInts(recordIDs));

    // Serialize existing points
    for (size_t i = 0; i < numChildren; ++i) {
        Storable::appendData(data, points[i].serialize(config));
    }
    // Pad out for empty slots
    Storable::appendData(data, std::vector<char>((config->maxChildren - numChildren) * Point::getSerializedSize(config), 0));

    return data;
}

TreeLeafNode TreeLeafNode::deserialize(GlobalParameters* config, const std::vector<char>& data) {
    if (data.size() < TreeLeafNode::getSerializedSize(config)) {
        throw std::invalid_argument("Data size is too small for TreeLeafNode deserialization.");
    }
    // Deserialize the base class first
    TreeNode baseNode = TreeNode::deserialize(config, data);
    int offset = TreeNode::getSerializedSize(config);
    
    // Prepare vectors for blockIDs, recordIDs and points
    std::vector<int> blockIDs = Storable::deserializeInts(data, offset, config->maxChildren);
    offset += config->maxChildren * sizeof(int);
    std::vector<int> recordIDs = Storable::deserializeInts(data, offset, config->maxChildren);
    offset += config->maxChildren * sizeof(int);
    
    std::vector<Point> points;
    for (int i = 0; i < config->maxChildren; ++i) {
        std::vector<char> pointData(data.begin() + offset, data.begin() + offset + Point::getSerializedSize(config));
        points.push_back(Point::deserialize(config, pointData));
        offset += Point::getSerializedSize(config);
    }
    printf("Final offset: %d\n", offset);
    return TreeLeafNode(config, baseNode.getID(), baseNode.getLevel(), baseNode.getParentID(), baseNode.getBoundingBox(), points, blockIDs, recordIDs);
}

int TreeLeafNode::getSerializedSize(GlobalParameters* config) {
    return TreeNode::getSerializedSize(config) + config->maxChildren * (
        sizeof(int) + // blockIDs
        sizeof(int) + // recordIDs
        Point::getSerializedSize(config) // points
    );
}