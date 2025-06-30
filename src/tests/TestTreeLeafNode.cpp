#include <gtest/gtest.h>
#include "treeleafnode.h"


// For every test other than the constructor, make a standrad test node
TreeLeafNode* createTestNode(GlobalParameters* config) {
    if (config->maxChildren < 3) {
        throw std::invalid_argument("maxChildren must be at least 3.");
    }
    int id = 1;
    int level = 0;
    int parentID = -1;
    Region boundingBox(std::vector<double>{0.0, 0.0}, std::vector<double>{1.0, 1.0});
    
    std::vector<Point> points = {Point({0.1, 0.1}), Point({0.1, 0.2}), Point({0.2, 0.2})};
    std::vector<int> blockIDs = {10, 20, 10};
    std::vector<int> recordIDs = {100, 200, 101};

    return new TreeLeafNode(config, id, level, parentID, boundingBox, points, blockIDs, recordIDs);
}

TEST(TreeLeafNodeTest, ConstructorAndGetters) {
    GlobalParameters* config = new GlobalParameters;
    config->dimensions = 2;
    config->maxChildren = 4;

    int id = 1;
    int level = 0;
    int parentID = -1;
    Region boundingBox(std::vector<double>{0.0, 0.0}, std::vector<double>{1.0, 1.0});
    
    std::vector<Point> points = {Point({0.1, 0.1}), Point({0.1, 0.2}), Point({0.2, 0.2})};
    std::vector<int> blockIDs = {10, 20, 10};
    std::vector<int> recordIDs = {100, 200, 101};

    TreeLeafNode node(config, id, level, parentID, boundingBox, points, blockIDs, recordIDs);
    TreeNode* nodePtr = &node;

    EXPECT_EQ(node.getID(), id);
    EXPECT_EQ(node.getLevel(), level);
    EXPECT_TRUE(node.isLeaf());
    EXPECT_TRUE(nodePtr->isLeaf()); // Check through base class pointer
    EXPECT_EQ(node.getParentID(), parentID);
    EXPECT_EQ(node.getBoundingBox(), boundingBox);
    EXPECT_EQ(node.getNumChildren(), 3);
    EXPECT_EQ(node.getPoints().size(), config->maxChildren); // Should have maxChildren size

    delete config;
}

TEST(TreeLeafNodeTest, AddPoint) {
    GlobalParameters* config = new GlobalParameters;
    config->dimensions = 2;
    config->maxChildren = 4;
    TreeLeafNode node = *createTestNode(config);

    Point newPoint({0.3, 0.3});
    node.addPoint(config, newPoint, 30, 300);

    // added fourth child 
    EXPECT_EQ(node.getNumChildren(), 4);
    EXPECT_EQ(node.getPoints()[3].getCoordinates(), newPoint.getCoordinates());
    EXPECT_EQ(node.getBlockIDs()[3], 30);
    EXPECT_EQ(node.getRecordIDs()[3], 300);

    EXPECT_THROW(node.addPoint(config, Point({0.4, 0.4}), 40, 400), std::overflow_error);

    // Reset
    node = *createTestNode(config);
    // Point of the same location not allowed
    EXPECT_THROW(node.addPoint(config, Point({0.1, 0.1}), 123, 500), std::invalid_argument); 
    // Negative Block ID and Record ID not allowed
    EXPECT_THROW(node.addPoint(config, Point({0.5, 0.5}), -1, 600), std::invalid_argument);
    EXPECT_THROW(node.addPoint(config, Point({0.6, 0.6}), 50, -1), std::invalid_argument);

    delete config;
}

TEST(TreeLeafNodeTest, AddPoints) {
    GlobalParameters* config = new GlobalParameters;
    config->dimensions = 2;
    config->maxChildren = 4;
    TreeLeafNode node = *createTestNode(config);

    std::vector<Point> newPoints = {Point({0.3, 0.3}), Point({0.4, 0.4})};
    std::vector<int> newBlockIDs = {30, 40};
    std::vector<int> newRecordIDs = {300, 400};

    // Node contains 3, max is 4
    EXPECT_THROW(node.addPoints(config, newPoints, newBlockIDs, newRecordIDs), std::overflow_error);

    GlobalParameters* config2 = new GlobalParameters;
    config2->dimensions = 2;
    config2->maxChildren = 5;
    node = *createTestNode(config2); // Create a new node with maxChildren = 5

    node.addPoints(config2, newPoints, newBlockIDs, newRecordIDs);

    EXPECT_EQ(node.getNumChildren(), 5);
    EXPECT_EQ(node.getPoints()[3].getCoordinates(), newPoints[0].getCoordinates());
    EXPECT_EQ(node.getBlockIDs()[3], newBlockIDs[0]);
    EXPECT_EQ(node.getRecordIDs()[3], newRecordIDs[0]);
    EXPECT_EQ(node.getPoints()[4].getCoordinates(), newPoints[1].getCoordinates());
    EXPECT_EQ(node.getBlockIDs()[4], newBlockIDs[1]);
    EXPECT_EQ(node.getRecordIDs()[4], newRecordIDs[1]);

    delete config;
    delete config2;
}

TEST(TreeLeafNodeTest, FindPoint) {
    GlobalParameters* config = new GlobalParameters;
    config->dimensions = 2;
    config->maxChildren = 4;
    TreeLeafNode node = *createTestNode(config);

    auto result = node.findPoint(Point({0.1, 0.1}));
    EXPECT_EQ(result.first, 10);
    EXPECT_EQ(result.second, 100);

    result = node.findPoint(Point({0.2, 0.2}));
    EXPECT_EQ(result.first, 10);
    EXPECT_EQ(result.second, 101);

    result = node.findPoint(Point({0.3, 0.3}));
    EXPECT_EQ(result.first, -1);
    EXPECT_EQ(result.second, -1); // Not found

    delete config;
}

TEST(TreeLeafNodeTest, RangeQuery) {
    GlobalParameters* config = new GlobalParameters;
    config->dimensions = 2;
    config->maxChildren = 4;
    TreeLeafNode node = *createTestNode(config);

    // Query that overlaps with two points
    AbstractBoundedClass* queryRegion = new Region({0.0, 0.0}, {0.1, 0.2});
    auto results = node.rangeQuery(*queryRegion);
    delete queryRegion;

    EXPECT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].first, 10);
    EXPECT_EQ(results[0].second, 100);
    EXPECT_EQ(results[1].first, 20);
    EXPECT_EQ(results[1].second, 200);

    // Query that does not overlap with any points
    queryRegion = new Region({0.5, 0.5}, {0.6, 0.6});
    results = node.rangeQuery(*queryRegion);
    delete queryRegion;

    EXPECT_EQ(results.size(), 0); // No results

    delete config;
}

TEST(TreeLeafNodeTest, RemovePointByID) {
    GlobalParameters* config = new GlobalParameters;
    config->dimensions = 2;
    config->maxChildren = 4;
    TreeLeafNode node = *createTestNode(config);

    // Remove a point by blockID and recordID
    int removed = node.removePoint(10, 100);
    EXPECT_EQ(removed, 0);
    EXPECT_EQ(node.getNumChildren(), 2); // One point removed

    // Try to remove a non-existing point
    removed = node.removePoint(99, 999);
    EXPECT_EQ(removed, -1); // Not found

    delete config;
}

TEST(TreeLeafNodeTest, RemovePointByPoint) {
    GlobalParameters* config = new GlobalParameters;
    config->dimensions = 2;
    config->maxChildren = 4;
    TreeLeafNode node = *createTestNode(config);

    // Remove a point by Point object
    int removed = node.removePoint(Point({0.1, 0.1}));
    EXPECT_EQ(removed, 0);
    EXPECT_EQ(node.getNumChildren(), 2); // One point removed

    // Try to remove a non-existing point
    removed = node.removePoint(Point({0.9, 0.9}));
    EXPECT_EQ(removed, -1); // Not found

    delete config;
}

TEST(TreeLeafNodeTest, GetSerializedSize) {
    for (int dimensions = 1; dimensions <= 20; ++dimensions) {
        for (int maxChildren = 1; maxChildren <= 1000; ++maxChildren) {
            GlobalParameters* config = new GlobalParameters;
            config->dimensions = dimensions;
            config->maxChildren = maxChildren;
            EXPECT_EQ(TreeLeafNode::getSerializedSize(config), 
                        TreeNode::getSerializedSize(config) + config->maxChildren * ( // base node + M *
                            Point::getSerializedSize(config) + // Points
                            sizeof(int) * 2 // blockIDs and recordIDs
                        ));
            delete config;
        }
    }
}

TEST(TreeLeafNodeTest, SerializeAndDeserialize) {
    GlobalParameters* config = new GlobalParameters;
    config->dimensions = 2;
    config->maxChildren = 4;
    TreeLeafNode node = *createTestNode(config);

    std::vector<char> serializedData = node.serialize(config);
    // VERY IMPORTANT!!!
    EXPECT_EQ(serializedData.size(), TreeLeafNode::getSerializedSize(config));

    TreeLeafNode deserializedNode = TreeLeafNode::deserialize(config, serializedData);

    EXPECT_EQ(deserializedNode.getID(), node.getID());
    EXPECT_EQ(deserializedNode.getLevel(), node.getLevel());
    EXPECT_TRUE(deserializedNode.isLeaf());
    EXPECT_EQ(deserializedNode.getParentID(), node.getParentID());
    EXPECT_EQ(deserializedNode.getBoundingBox(), node.getBoundingBox());
    EXPECT_EQ(deserializedNode.getNumChildren(), node.getNumChildren());
    EXPECT_EQ(deserializedNode.getPoints().size(), node.getPoints().size());

    for (size_t i = 0; i < node.getNumChildren(); ++i) {
        EXPECT_EQ(deserializedNode.getPoints()[i].getCoordinates(), node.getPoints()[i].getCoordinates());
        EXPECT_EQ(deserializedNode.getBlockIDs()[i], node.getBlockIDs()[i]);
        EXPECT_EQ(deserializedNode.getRecordIDs()[i], node.getRecordIDs()[i]);
    }

    delete config;
}