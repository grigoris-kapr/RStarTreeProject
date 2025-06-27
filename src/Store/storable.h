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

    /* 
    =================================================================
                    NEED TO IMPLEMENT EACH TIME
    =================================================================
    Deserialize the object
    static Storable deserialize(const std::vector<char>& data) = 0;

    Get the size of the serialized object
    static int getSerializedSize() = 0;
    */

    static std::vector<char> serializeInt(int value);
    static int deserializeInt(const std::vector<char>& data, size_t offset = 0);
    static std::vector<char> serializeLongLong(long long value);
    static long long deserializeLongLong(const std::vector<char>& data, size_t offset = 0);
    static std::vector<char> serializeDouble(double value);
    static double deserializeDouble(const std::vector<char>& data, size_t offset = 0);
    // Vector (de)serialization methods for performance
    static std::vector<char> serializeInts(const std::vector<int>& vec);
    static std::vector<int> deserializeInts(const std::vector<char>& data, size_t offset = 0, size_t count = 0);
    static std::vector<char> serializeLongLongs(const std::vector<long long>& vec);
    static std::vector<long long> deserializeLongLongs(const std::vector<char>& data, size_t offset = 0, size_t count = 0);
    static std::vector<char> serializeDoubles(const std::vector<double>& vec);
    static std::vector<double> deserializeDoubles(const std::vector<char>& data, size_t offset = 0, size_t count = 0);
    
    static void appendData(std::vector<char>& data, const std::vector<char>& additionalData);
};

#endif // STORABLE_H