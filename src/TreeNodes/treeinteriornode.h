#ifndef TREEINTERIORNODE_H
#define TREEINTERIORNODE_H

#include "treenode.h"

class TreeInteriorNode:   public TreeNode{
private:
    int maxChildren; // Maximum number of children this node can have
    int* childrenIDs; // Array of child node IDs, size determined by maxChildren upon creation
    int numChildren; // Current number of children
    Region* childrenBoundingBoxes; // Array of bounding boxes for each child node, size determined by maxChildren upon creation

protected:
    void updateBoundingBox();

public:
    TreeInteriorNode (int id, int maxChildren, int level, int parentID, const Region& rectangle, int* childrenIDs = nullptr, Region* childrenBoundingBoxes = nullptr);
    ~TreeInteriorNode ();
    const int* getChildrenIDs() const { return childrenIDs; }

    std::vector<char> serialize() const override;
    static TreeInteriorNode deserialize(const std::vector<char>& data, int maxChildren, int dimensions);
    static int getSerializedSize(int maxChildren, int dimensions);

    // Interface methods
    void addChild(int childID, const Region& childBoundingBox);
    void addChildren(const std::vector<int>& childrenIDs, const std::vector<Region>& childrenBoundingBoxes);
    int removeChild(int childID);
    
};

#endif // TREEINTERIORNODE_H