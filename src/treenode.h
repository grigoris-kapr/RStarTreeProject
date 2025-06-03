#ifndef TREENODE_H
#define TREENODE_H

#include <vector>
#include "abstractBoundedClass.h"
#include "region.h"
#include "storable.h"

class TreeNode: public Storable{
private:
    int id; // Unique identifier for the node
    int level; // Level of the node in the tree
    int parentID; // ID of the parent node
    int* childrenIDs; // Array of child node IDs, size determined by maxChildren upon creation
    int numChildren; // Current number of children
    Region rectangle; // Bounding box of the node


public:
    TreeNode (int id, int maxChildren, int level = 0, int parentID = -1);
    TreeNode (int id, int maxChildren, int level, int parentID, const Region& rectangle, int* childrenIDs = nullptr);
    ~TreeNode ();
    bool isLeaf() const {
        return level == 0; // Bottom-up counting for constant leaf level
    }
    int getID() const { return id; }
    int getLevel() const { return level; }
    int getParentID() const { return parentID; }
    const Region getBoundingBox() const { return rectangle; }
    const int* getChildrenIDs() const { return childrenIDs; }

    std::vector<char> serialize() const override;
    static TreeNode deserialize(const std::vector<char>& data, int maxChildren);
    

};

TreeNode ::TreeNode (int id, int maxChildren, int level = 0, int parentID = -1) {
    this->id = id;
    this->level = level; // Default level, can be set later
    this->parentID = parentID; // Default parent ID, indicating no parent

    this->childrenIDs = new int[maxChildren]; // Allocate memory for child IDs
    this->numChildren = 0; // Initialize number of children to 0
    for (int i = 0; i < maxChildren; ++i) {
        childrenIDs[i] = -1; // Initialize child IDs to -1 (indicating no child)
    }
    this->rectangle = Region(); // Initialize the bounding box as an empty region
}

TreeNode ::TreeNode (int id, int maxChildren, int level, int parentID, const Region& rectangle, int* childrenIDs) {
    this->id = id;
    this->level = level;
    this->parentID = parentID;

    this->childrenIDs = new int[maxChildren];
    if(childrenIDs == nullptr) {
        throw std::invalid_argument("childrenIDs cannot be null when initializing TreeNode. Use the other constructor if no children are present.");
    }
    this->numChildren = 0; // Initialize number of children to 0
    for (int i = 0; i < maxChildren; ++i) {
        this->childrenIDs[i] = childrenIDs[i];
        if (childrenIDs[i] != -1) {
            this->numChildren++; // Count the number of valid children
        }
    }
    this->rectangle = rectangle; // Set the bounding box
}

TreeNode ::~TreeNode () {
}




#endif // TREENODE_H