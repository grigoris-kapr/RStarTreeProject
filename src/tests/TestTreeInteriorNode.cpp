#include <gtest/gtest.h>
#include "treeinteriornode.h"

TEST(TreeInteriorNodeTest, ConstructorAndGetters) {
    int maxChildren = 3;
    int id = 1;
    int level = 1;
    int parentID = -1;
    Region rectangle(std::vector<double>{0.0, 0.0}, std::vector<double>{1.0, 1.0}); // A 2D region from (0,0) to (1,1)
    
    int childrenIDs[] = {2, 3, -1}; // -1 indicates no child
    Region childrenBoundingBoxes[] = {
        Region(std::vector<double> {0.0, 0.0}, std::vector<double> {0.5, 0.5}),
        Region(std::vector<double> {0.5, 0.5}, std::vector<double> {1.0, 1.0}),
        Region() // Last one is empty
    }; 

    TreeInteriorNode node(id, maxChildren, level, parentID, rectangle, childrenIDs, childrenBoundingBoxes);

    // Make sure we're not running into same pointer or something
    rectangle = Region(std::vector<double>{0.0, 0.0}, std::vector<double>{1.0, 1.0});

    EXPECT_EQ(node.getID(), id);
    EXPECT_EQ(node.getLevel(), level);
    EXPECT_EQ(node.getParentID(), parentID);
    EXPECT_EQ(node.getBoundingBox(), rectangle);
    EXPECT_EQ(node.getChildrenIDs()[0], 2);
    EXPECT_EQ(node.getChildrenIDs()[1], 3);
    EXPECT_EQ(node.getChildrenIDs()[2], -1);
}