#ifndef TREELEAFNODE_H
#define TREELEAFNODE_H

#include "treenode.h"
#include <vector>
#include "point.h"

class TreeLeafNode : public TreeNode {
private:
    // Store the blockID and recordID for each datapoint
    // Use the point coordinates to identify the datapoint
    std::vector<int> blockIDs; // -1 for empty slots
    std::vector<int> recordIDs; // -1 for empty slots
    std::vector<Point> points; // Point() for empty slots
    std::string printPointInfo(int i) const;
    int findPointIndex(const Point& point) const; // Returns the index of the point if found, otherwise -1
public:
    TreeLeafNode(int id, int maxChildren, int level, int parentID, const Region& boundingBox, const std::vector<Point>& points, const std::vector<int>& blockIDs, const std::vector<int>& recordIDs);
    ~TreeLeafNode() = default;

    // Interface methods
    void addPoint(const Point& point, int blockID, int recordID);
    void addPoints(const std::vector<Point>& points, const std::vector<int>& blockIDs, const std::vector<int>& recordIDs); // For tree initialization
    std::pair<int, int> findPoint(const Point& point) const; // <blockID, recordID> or (-1, -1) if not found
    std::vector<std::pair<int, int>> rangeQuery(const AbstractBoundedClass& query) const; // <blockID, recordID> 
    int removePoint(int blockID, int recordID);
    int removePoint(const Point& point);

    // Getters
    const std::vector<Point>& getPoints() const { return points; }
    const std::vector<int>& getBlockIDs() const { return blockIDs; }
    const std::vector<int>& getRecordIDs() const { return recordIDs; }

    // Serialization
    std::vector<char> serialize() const override;
    static TreeLeafNode deserialize(const std::vector<char>& data, int maxChildren, int dimensions);
    static int getSerializedSize(int maxChildren, int dimensions);
};

#endif // TREELEAFNODE_H