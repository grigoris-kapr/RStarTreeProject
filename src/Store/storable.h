#ifndef STORABLE_H
#define STORABLE_H

#include <vector>
#include "globalparameters.h"
#include <stdexcept>

class Storable {
    // Virtual base class for all storable objects.
    // TODO: Polymorphism with desetialize and getSerializedSize.
    // This class represents an **abstract** storable object that can be serialized and deserialized.
    // It is intended to be used as a base class for objects that need to be stored in a database or file.
public:
    ~Storable() = default;
    
    // Default implementations. Must be overridden in derived classes.
    virtual std::vector<char> serialize(GlobalParameters* config) const { 
        throw std::runtime_error("Serialize not implemented for this type.");
    }
    static Storable deserialize(GlobalParameters* config, const std::vector<char>& data) { 
        throw std::runtime_error("Deserialize not implemented for this type.");
    }
    static int getSerializedSize(GlobalParameters* config) {
        throw std::runtime_error("GetSerializedSize not implemented for this type.");
    }

    /*
        DATATYPE HANDLING
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