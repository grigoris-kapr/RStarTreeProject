#include <gtest/gtest.h>
#include "treeinteriornode.h"

TEST(TreeInteriorNodeTest, ConstructorAndGetters) {
    int maxChildren = 3;
    int id = 1;
    int level = 1;
    int parentID = -1;
    Region rectangle(std::vector<double>{0.0, 0.0}, std::vector<double>{1.0, 1.0}); // A 2D region from (0,0) to (1,1)
    
    std::vector<int> childrenIDs = {2, 3, -1}; // -1 indicates no child
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
    EXPECT_FALSE(node.isLeaf());
    EXPECT_EQ(node.getParentID(), parentID);
    EXPECT_EQ(node.getBoundingBox(), rectangle);
    EXPECT_EQ(node.getNumChildren(), 2);
    EXPECT_EQ(node.getChildrenIDs()[0], 2);
    EXPECT_EQ(node.getChildrenIDs()[1], 3);
    EXPECT_EQ(node.getChildrenIDs()[2], -1);
}

TEST(TreeInteriorNodeTest, AddChild) {
    int maxChildren = 3;
    int id = 1;
    int level = 1;
    int parentID = -1;
    Region rectangle(std::vector<double>{0.0, 0.0}, std::vector<double>{1.0, 1.0});
    
    std::vector<int> childrenIDs = {2, 3, -1}; // -1 indicates no child
    Region childrenBoundingBoxes[] = {
        Region(std::vector<double> {0.0, 0.0}, std::vector<double> {0.5, 0.5}),
        Region(std::vector<double> {0.5, 0.5}, std::vector<double> {1.0, 1.0}),
        Region() // Last one is empty
    }; 

    TreeInteriorNode node(id, maxChildren, level, parentID, rectangle, childrenIDs, childrenBoundingBoxes);

    Region newChildRegion(std::vector<double>{0.25, 0.25}, std::vector<double>{0.75, 0.75});
    node.addChild(4, newChildRegion);

    // Successful addition
    EXPECT_EQ(node.getChildrenIDs()[2], 4);
    EXPECT_EQ(node.getNumChildren(), 3);
    // Unsuccessful addition (already full)
    EXPECT_THROW(node.addChild(5, newChildRegion), std::overflow_error); 
    EXPECT_EQ(node.getNumChildren(), 3);
}

TEST(TreeInteriorNodeTest, RemoveChild) {
    int maxChildren = 3;
    int id = 1;
    int level = 1;
    int parentID = -1;
    Region rectangle(std::vector<double>{0.0, 0.0}, std::vector<double>{1.0, 1.0});
    
    std::vector<int> childrenIDs = {2, 3, -1}; // -1 indicates no child
    Region childrenBoundingBoxes[] = {
        Region(std::vector<double> {0.0, 0.0}, std::vector<double> {0.5, 0.5}),
        Region(std::vector<double> {0.5, 0.5}, std::vector<double> {1.0, 1.0}),
        Region() // Last one is empty
    }; 

    TreeInteriorNode node(id, maxChildren, level, parentID, rectangle, childrenIDs, childrenBoundingBoxes);

    // First, remove a child that exists from the two children
    EXPECT_EQ(node.removeChild(2), 0);
    EXPECT_EQ(node.getChildrenIDs()[0], 3);
    EXPECT_EQ(node.getChildrenIDs()[1], -1); 
    EXPECT_EQ(node.getChildrenIDs()[2], -1); 
    EXPECT_EQ(node.getNumChildren(), 1);
    // Then, remove a child that doesn't exist
    EXPECT_EQ(node.removeChild(4), -1);
    EXPECT_EQ(node.getChildrenIDs()[0], 3);
    EXPECT_EQ(node.getChildrenIDs()[1], -1); 
    EXPECT_EQ(node.getChildrenIDs()[2], -1); 
    EXPECT_EQ(node.getNumChildren(), 1);
    // Then, remove the last child
    EXPECT_EQ(node.removeChild(3), 0);
    EXPECT_EQ(node.getChildrenIDs()[0], -1);
    EXPECT_EQ(node.getChildrenIDs()[1], -1); 
    EXPECT_EQ(node.getChildrenIDs()[2], -1); 
    EXPECT_EQ(node.getNumChildren(), 0);
    // Finally, try to remove from an empty node
    EXPECT_EQ(node.removeChild(3), -1);
    EXPECT_EQ(node.getChildrenIDs()[0], -1);
    EXPECT_EQ(node.getChildrenIDs()[1], -1); 
    EXPECT_EQ(node.getChildrenIDs()[2], -1);
    EXPECT_EQ(node.getNumChildren(), 0);
}

TEST(TreeInteriorNodeTest, BoundingBoxUpdate) {
    int maxChildren = 3;
    int id = 1;
    int level = 1;
    int parentID = -1;
    Region rectangle(std::vector<double>{0.0, 0.0}, std::vector<double>{1.0, 1.0});
    
    std::vector<int> childrenIDs = {2, 3, -1}; // -1 indicates no child
    Region childrenBoundingBoxes[] = {
        Region(std::vector<double> {0.0, 0.0}, std::vector<double> {0.5, 0.5}),
        Region(std::vector<double> {0.5, 0.5}, std::vector<double> {1.0, 1.0}),
        Region() // Last one is empty
    }; 

    TreeInteriorNode node(id, maxChildren, level, parentID, rectangle, childrenIDs, childrenBoundingBoxes);

    // Add a child that expands the bounding box
    Region newChildRegion(std::vector<double>{-0.5, -0.5}, std::vector<double>{1.5, 1.5});
    node.addChild(4, newChildRegion);
    Region rectangleAfterAddition = Region::boundingBox(std::vector<AbstractBoundedClass*>{&rectangle, &newChildRegion});
    
    // The bounding box should now encompass the new child
    EXPECT_EQ(node.getBoundingBox(), rectangleAfterAddition);
    // Overfill to check it doesn't change the bounding box
    EXPECT_THROW(node.addChild(5, newChildRegion), std::overflow_error);
    EXPECT_EQ(node.getBoundingBox(), rectangleAfterAddition);
    // Remove the child and check if the bounding box updates correctly
    node.removeChild(4);
    EXPECT_EQ(node.getBoundingBox(), rectangle); // Should revert to the original bounding box
    // Remove all children and check for empty bounding box
    node.removeChild(2);
    node.removeChild(3);
    EXPECT_EQ(node.getBoundingBox(), Region()); // Should be an empty region
    // Underfill to check it doesn't change the bounding box
    EXPECT_EQ(node.removeChild(3), -1);
    EXPECT_EQ(node.getBoundingBox(), Region()); // Should still be an empty region
}

TEST(TreeInteriorNodeTest, GetSerialisedSize) {
    // Code should be quick so test all options from 1 to some high number
    for (int dimensions = 1; dimensions <= 20; ++dimensions) {
        for(int maxChildren = 1; maxChildren <= 1000; ++maxChildren) {
            EXPECT_EQ(TreeInteriorNode::getSerializedSize(maxChildren, dimensions), 
                      TreeNode::getSerializedSize(dimensions) + 
                      // Plus one ID and one Region per child
                      maxChildren * (sizeof(int) + Region::getSerializedSize(dimensions)));
        }
    }
}

TEST(TreeInteriorNodeTest, SerializeAndDeserialize) {
    int maxChildren = 3;
    int id = 1;
    int level = 1;
    int parentID = -1;
    Region rectangle(std::vector<double>{0.0, 0.0}, std::vector<double>{1.0, 1.0});
    
    std::vector<int> childrenIDs = {2, 3, -1}; // -1 indicates no child
    Region childrenBoundingBoxes[] = {
        Region(std::vector<double> {0.0, 0.0}, std::vector<double> {0.5, 0.5}),
        Region(std::vector<double> {0.5, 0.5}, std::vector<double> {1.0, 1.0}),
        Region() // Last one is empty
    }; 

    TreeInteriorNode originalNode(id, maxChildren, level, parentID, rectangle, childrenIDs, childrenBoundingBoxes);
    
    std::vector<char> serializedData = originalNode.serialize();
    // VERY IMPORTANT!!!
    EXPECT_EQ(serializedData.size(), TreeInteriorNode::getSerializedSize(maxChildren, rectangle.getStart().size()));
    
    TreeInteriorNode deserializedNode = TreeInteriorNode::deserialize(serializedData, maxChildren, rectangle.getStart().size());
    
    EXPECT_EQ(originalNode.getID(), deserializedNode.getID());
    EXPECT_EQ(originalNode.getLevel(), deserializedNode.getLevel());
    EXPECT_EQ(originalNode.getParentID(), deserializedNode.getParentID());
    EXPECT_EQ(originalNode.getBoundingBox(), deserializedNode.getBoundingBox());
    EXPECT_EQ(originalNode.getChildrenIDs(), deserializedNode.getChildrenIDs());
}

TEST(TreeInteriorNodeTest, RangeQuery) {
    int maxChildren = 3;
    int id = 1;
    int level = 1;
    int parentID = -1;
    Region rectangle(std::vector<double>{0.0, 0.0}, std::vector<double>{1.0, 1.0});
    
    std::vector<int> childrenIDs = {2, 3, -1}; // -1 indicates no child
    Region childrenBoundingBoxes[] = {
        Region(std::vector<double> {0.0, 0.0}, std::vector<double> {0.5, 0.5}),
        Region(std::vector<double> {0.5, 0.5}, std::vector<double> {1.0, 1.0}),
        Region() // Last one is empty
    }; 

    TreeInteriorNode node(id, maxChildren, level, parentID, rectangle, childrenIDs, childrenBoundingBoxes);

    // Query that overlaps with both children
    Region queryRegion(std::vector<double>{0.25, 0.25}, std::vector<double>{0.75, 0.75});
    std::vector<int> result = node.rangeQuery(queryRegion);
    EXPECT_EQ(result.size(), 2);
    // order is irrelevant
    EXPECT_TRUE(result[0] == 2 || result[1] == 2);
    EXPECT_TRUE(result[0] == 3 || result[1] == 3);

    // Query that only overlaps with the first child
    queryRegion = Region(std::vector<double>{-0.25, -0.25}, std::vector<double>{0.25, 0.25});
    result = node.rangeQuery(queryRegion);
    
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 2);

    // Query that does not overlap with any children
    queryRegion = Region(std::vector<double>{2.0, 2.0}, std::vector<double>{3.0, 3.0});
    result = node.rangeQuery(queryRegion);

    EXPECT_TRUE(result.empty());

    // Test query on full node
    node.addChild(4, Region(std::vector<double>{0.1, 0.1}, std::vector<double>{0.2, 0.2}));
    queryRegion = Region(std::vector<double>{0.0, 0.0}, std::vector<double>{1.0, 1.0});
    result = node.rangeQuery(queryRegion);

    EXPECT_EQ(result.size(), 3);
    EXPECT_TRUE(result[0] == 2 || result[1] == 2 || result[2] == 2);
    EXPECT_TRUE(result[0] == 3 || result[1] == 3 || result[2] == 3);
    EXPECT_TRUE(result[0] == 4 || result[1] == 4 || result[2] == 4);
}