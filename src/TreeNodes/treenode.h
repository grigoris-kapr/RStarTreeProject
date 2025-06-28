#ifndef TREENODE_H
#define TREENODE_H

#include <vector>
#include "abstractBoundedClass.h"
#include "region.h"
#include "storable.h"

// Class implementing the common parts of interior and leaf nodes in the tree.
class TreeNode: public Storable{
protected:
    int id; // Unique identifier for the node
    int level; // Level of the node in the tree
    int parentID; // ID of the parent node
    int numChildren = 0; // Number of children
    int maxChildren = 0; // Maximum number of children this node can have
    Region boundingBox; // Bounding box of the node

    void updateBoundingBox() { return; } // Pure virtual function but can't make class abstract

public:
    TreeNode (int id, int maxChildren, int level, int parentID, const Region& boundingBox);
    ~TreeNode ();

    bool isLeaf() const { return false; } // Left as-is for interior nodes; leaf nodes will override this method

    int getID() const { return id; }
    int getLevel() const { return level; }
    int getParentID() const { return parentID; }
    int getNumChildren() const { return numChildren; }
    const Region getBoundingBox() const { return boundingBox; }

    std::vector<char> serialize() const override;
    static TreeNode deserialize(const std::vector<char>& data, int maxChildren, int dimensions);
    static int getSerializedSize(int dimensions);

};

#endif // TREENODE_H