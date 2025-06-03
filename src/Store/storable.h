#ifndef STORABLE_H
#define STORABLE_H

#include <vector>

class Storable {
    // This class represents an abstract storable object that can be serialized and deserialized.
    // It is intended to be used as a base class for objects that need to be stored in a database or file.
public:
    virtual ~Storable() = default;
    
    // Serialize the object to a string representation
    virtual std::vector<char> serialize() const = 0;

    // NEED TO IMPLEMENT EACH TIME: Deserialize the object from a string representation
    // static Storable deserialize(const std::vector<char>& data) = 0;

    static std::vector<char> serializeInt(int value) {
        std::vector<char> data(sizeof(int));
        for (size_t i = 0; i < sizeof(int); ++i) {
            data[i] = static_cast<char>((value >> (i * 8)) & 0xFF);
        }
        return data;
    }

    static int deserializeInt(const std::vector<char>& data, size_t offset = 0) {
        if (data.size() < offset + sizeof(int)) {
            throw std::invalid_argument("Data size is too small for integer deserialization.");
        }
        int value = 0;
        for (size_t i = 0; i < sizeof(int); ++i) {
            value |= (static_cast<unsigned char>(data[offset + i]) << (i * 8));
        }
        return value;
    }

    static std::vector<char> serializeDouble(double value) {
        std::vector<char> data(sizeof(double));
        for (size_t i = 0; i < sizeof(double); ++i) {
            data[i] = static_cast<char>((reinterpret_cast<const unsigned char*>(&value))[i]);
        }
        return data;
    }

    static double deserializeDouble(const std::vector<char>& data, size_t offset = 0) {
        if (data.size() < offset + sizeof(double)) {
            throw std::invalid_argument("Data size is too small for double deserialization.");
        }
        double value = 0.0;
        for (size_t i = 0; i < sizeof(double); ++i) {
            value += static_cast<unsigned char>(data[offset + i]) << (i * 8);
        }
        return value;
    }

    static void appendData(std::vector<char>& data, const std::vector<char>& additionalData) {
        if (data.empty()) {
            throw std::invalid_argument("Data cannot be empty.");
        }
        if (additionalData.empty()) {
            throw std::invalid_argument("Additional data cannot be empty.");
        }
        data.insert(data.end(), additionalData.begin(), additionalData.end());
    }
};

#endif // STORABLE_H