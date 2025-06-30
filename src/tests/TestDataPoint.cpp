#include <gtest/gtest.h>
#include "datapoint.h"

void testData(const DataPoint& point, const std::vector<char>& expectedData) {
    std::vector<char> pointData = point.getData();
    EXPECT_EQ(pointData.size(), MAX_DATA_SIZE); // defined in datapoint.h
    for (size_t i = 0; i < expectedData.size(); ++i) {
        EXPECT_EQ(pointData[i], expectedData[i]);
    }  
    for (size_t i = expectedData.size(); i < MAX_DATA_SIZE; ++i) {
        EXPECT_EQ(pointData[i], '\0'); // Ensure the rest, if any are null characters
    }
}

TEST(DataPointTest, CoordsConstructorAndGetters) {
    std::vector<double> coords = {1.0, 2.0, 3.0};
    std::vector<char> data = {'a', 'b', 'c'};
    long long id = 123456789;

    DataPoint point(coords, data, id);

    EXPECT_EQ(point.getPoint().getCoordinates(), coords);
    testData(point, data);
    EXPECT_EQ(point.getID(), id);
}

TEST(DataPointTest, PointConstructorAndGetters) {
    Point point(std::vector<double>{4.0, 5.0, 6.0});
    std::vector<char> data = {'x', 'y', 'z'};
    long long id = 987654321;

    DataPoint dataPoint(point, data, id);

    EXPECT_EQ(dataPoint.getPoint().getCoordinates(), point.getCoordinates());
    testData(dataPoint, data);
    EXPECT_EQ(dataPoint.getID(), id);
}

TEST(DataPointTest, SetData) {
    std::vector<double> coords = {7.0, 8.0, 9.0};
    std::vector<char> data = {'1', '2', '3'};
    long long id = 111222333;

    DataPoint point(coords, data, id);
    
    std::vector<char> newData = {'4', '5', '6'};
    point.setData(newData);

    testData(point, newData);
}

TEST(DataPointTest, MaxDataSize) {
    std::vector<double> coords = {10.0, 11.0, 12.0};
    std::vector<char> goodData(MAX_DATA_SIZE, 'x'); // exactly MAX_DATA_SIZE characters
    std::vector<char> badData(MAX_DATA_SIZE + 1, 'x'); // exactly MAX_DATA_SIZE+1 characters
    long long id = 444555666;

    EXPECT_THROW(DataPoint point(coords, badData, id), std::invalid_argument);
    DataPoint goodPoint(coords, goodData, id);
    testData(goodPoint, goodData);
}

TEST(DataPointTest, GetSerializedSize) {
    // Code should be quick so test all options from 0 (which makes no sense) to some high number
    for (int dimensions = 0; dimensions <= 100; ++dimensions) {
        GlobalParameters* config = new GlobalParameters;
        config->dimensions = dimensions;
        config->maxChildren = 12; // Irrelevant for this
        EXPECT_EQ(DataPoint::getSerializedSize(config), sizeof(long long) + MAX_DATA_SIZE + Point::getSerializedSize(config));
        delete config;
    }
}

TEST(DataPointTest, SerializeAndDeserialize) {
    GlobalParameters* config = new GlobalParameters;
    config->dimensions = 3;
    config->maxChildren = 12; // Irrelevant for this

    std::vector<double> coords = {13.0, 14.0, 15.0};
    std::vector<char> data(3, 'x');
    long long id = 123456789;

    printf("DBG\n");

    DataPoint original(coords, data, id);
    std::vector<char> serializedData = original.serialize(config);
    // VERY IMPORTANT!!!
    EXPECT_EQ(serializedData.size(), DataPoint::getSerializedSize(config));

    printf("DBG\n");
    
    DataPoint deserialized = DataPoint::deserialize(config, serializedData);
    
    EXPECT_EQ(deserialized.getPoint().getCoordinates(), coords);
    testData(deserialized, data);
    EXPECT_EQ(deserialized.getID(), id);

    delete config;
}