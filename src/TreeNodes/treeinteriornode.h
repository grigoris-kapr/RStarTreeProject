#ifndef TREEINTERIORNODE_H
#define TREEINTERIORNODE_H

#include "treenode.h"

class TreeInteriorNode: public TreeNode {
private:
    std::vector<int> childrenIDs; // Array of child node IDs, size determined by maxChildren upon creation
    Region* childrenBoundingBoxes; // Array of bounding boxes for each child node, size determined by maxChildren upon creation

protected:
    void updateBoundingBox();

public:
    TreeInteriorNode (GlobalParameters* config, int id, int level, int parentID, const Region& rectangle, std::vector<int> childrenIDs, Region* childrenBoundingBoxes = nullptr);
    ~TreeInteriorNode ();
    std::vector<int> getChildrenIDs() const { return childrenIDs; }

    std::vector<char> serialize(GlobalParameters* config) const override;
    static TreeInteriorNode deserialize(GlobalParameters* config, const std::vector<char>& data);
    static int getSerializedSize(GlobalParameters* config);

    // Interface methods
    void addChild(GlobalParameters* config, int childID, const Region& childBoundingBox);
    void addChildren(GlobalParameters* config, const std::vector<int>& childrenIDs, const std::vector<Region>& childrenBoundingBoxes);
    int removeChild(int childID);
    std::vector<int> rangeQuery(const AbstractBoundedClass& query) const;
    
};

#endif // TREEINTERIORNODE_H