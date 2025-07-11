#include "datapoint.h"


DataPoint::DataPoint(const std::vector<double>& coords, const std::vector<char>& data, long long id) {
    if (data.size() > MAX_DATA_SIZE) {
        throw std::invalid_argument("Data cannot exceed " + std::to_string(MAX_DATA_SIZE) + " characters.");
    }
    this->data = data;
    this->data.resize(MAX_DATA_SIZE, '\0'); // Ensure data is exactly MAX_DATA_SIZE characters long

    this->point = Point(coords);
    this->id = id;
}

DataPoint::DataPoint(const Point& point, const std::vector<char>& data, long long id) {
    if (data.size() > MAX_DATA_SIZE) {
        throw std::invalid_argument("Data cannot exceed " + std::to_string(MAX_DATA_SIZE) + " characters.");
    }
    this->data = data;
    this->data.resize(MAX_DATA_SIZE, '\0'); // Ensure data is exactly MAX_DATA_SIZE characters long

    this->point = point;
    this->id = id;
}

void DataPoint::setData(const std::vector<char>& newData) {
    if (newData.size() > MAX_DATA_SIZE) {
        throw std::invalid_argument("Data cannot exceed " + std::to_string(MAX_DATA_SIZE) + " characters.");
    }
    data = newData;
    data.resize(MAX_DATA_SIZE, '\0'); // Ensure data is exactly MAX_DATA_SIZE characters long
}

/*
=========================================
============== STORAGE ==================
=========================================
*/

std::vector<char> DataPoint::serialize(GlobalParameters* config) const {
    std::vector<char> data = Storable::serializeLongLong(id); // Start with the ID

    Storable::appendData(data, this->data); // Append the point data

    Storable::appendData(data, point.serialize(config)); // Append the point data at the end

    return data;
}

DataPoint DataPoint::deserialize(GlobalParameters* config, const std::vector<char>& data) {
    if (data.size() < getSerializedSize(config)) {
        throw std::invalid_argument("Data size is too small for DataPoint deserialization.");
    }

    long long id = Storable::deserializeLongLong(data, 0); // Deserialize the ID
    std::vector<char> dataPointData(data.begin() + sizeof(long long), data.begin() + sizeof(long long) + MAX_DATA_SIZE);
    
    std::vector<char> pointSerializedData(data.begin() + sizeof(long long) + MAX_DATA_SIZE, data.end());
    Point point = Point::deserialize(config, pointSerializedData); // Deserialize the point

    return DataPoint(point, dataPointData, id);

}

int DataPoint::getSerializedSize(GlobalParameters* config) {
    return sizeof(long long) + MAX_DATA_SIZE + Point::getSerializedSize(config);
}