#ifndef BUFFER_H
#define BUFFER_H

#include <string>

class Buffer {
private:
    int size; // Size of the buffer in number of elements
    void* data; // Pointer to the buffer data, can be any type

    // Prevent copying and assignment
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
public:
    Buffer(int size); // is size in memory or in number of elements?????
    ~Buffer();

    void parseOSMFile(const std::string& filename);
};


#endif // BUFFER_H