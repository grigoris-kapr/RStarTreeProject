#ifndef TREENODE_H
#define TREENODE_H

#include <vector>
#include "abstractBoundedClass.h"
#include "region.h"

class TreeNode {
private:
    int id; // Unique identifier for the node
    int level; // Level of the node in the tree
    int parentID; // ID of the parent node
    Region rectangle; // Bounding box of the node
    std::vector<int> childrenIDs; // IDs of child nodes


public:
    TreeNode (/* args */);
    ~TreeNode ();
    bool isLeaf() const {
        return level == 0; // Bottom-up counting for constant leaf level
    }
    int getID() const { return id; }
    int getLevel() const { return level; }
    int getParentID() const { return parentID; }
    const Region& getBoundingBox() const { return rectangle; }
    const std::vector<int>& getChildrenIDs() const { return childrenIDs; }
};

TreeNode ::TreeNode (/* args */) {

}

TreeNode ::~TreeNode () {
}




#endif // TREENODE_H