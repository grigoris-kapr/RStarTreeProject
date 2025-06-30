#include <gtest/gtest.h>
#include "point.h"

TEST(PointTest, ConstructorAndGetters) {
    GlobalParameters* config = new GlobalParameters;
    config->dimensions = 3;
    config->maxChildren = 12; // Irrelevant for this

    std::vector<double> coords = {1.0, 2.0, 3.0};
    Point p(coords);
    // coords, start, and end should all be the same.
    // start and end are for compatibility with AbstractBoundedClass
    EXPECT_EQ(p.getCoordinates(), coords);
    EXPECT_EQ(p.getStart(), coords);
    EXPECT_EQ(p.getEnd(), coords);
    // String formatting
    EXPECT_EQ(p.toString(config), "Point(1.000000, 2.000000, 3.000000)");

    delete config;
}

TEST(PointTest, EmptryToString) {
    GlobalParameters* config = new GlobalParameters;
    config->dimensions = 3;
    config->maxChildren = 12; // Irrelevant for this


    Point p = Point();
    EXPECT_EQ(p.toString(config), "Point()"); // Should handle empty coordinates gracefully

    delete config;
}
