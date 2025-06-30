#ifndef TREENODE_H
#define TREENODE_H

#include <vector>
#include "abstractBoundedClass.h"
#include "region.h"
#include "storable.h"

// Class implementing the common parts of interior and leaf nodes in the tree.
// Extends Storable
class TreeNode: public Storable{
protected:
    int id; // Unique identifier for the node
    int level; // Level of the node in the tree
    int parentID; // ID of the parent node
    int numChildren = 0; // Number of children
    Region boundingBox; // Bounding box of the node

    void updateBoundingBox() { return; } // Pure virtual function but can't make class abstract

public:
    TreeNode (int id, int level, int parentID, const Region& boundingBox);
    ~TreeNode ();

    bool isLeaf() const { return level == 0; } 
    
    int getID() const { return id; }
    int getLevel() const { return level; }
    int getParentID() const { return parentID; }
    int getNumChildren() const { return numChildren; }
    const Region getBoundingBox() const { return boundingBox; }

    std::vector<char> serialize(GlobalParameters* config) const override;
    static TreeNode deserialize(GlobalParameters* config, const std::vector<char>& data);
    static int getSerializedSize(GlobalParameters* config);

};

#endif // TREENODE_H