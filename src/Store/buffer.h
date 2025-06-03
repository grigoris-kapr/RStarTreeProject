#ifndef BUFFER_H
#define BUFFER_H


class AbstractBuffer {
private:
    int size; // Size of the buffer in number of elements
    void* data; // Pointer to the buffer data, can be any type

    // Prevent copying and assignment
    AbstractBuffer(const AbstractBuffer&) = delete;
    AbstractBuffer& operator=(const AbstractBuffer&) = delete;
public:
    AbstractBuffer(int size); // is size in memory or in number of elements?????
    ~AbstractBuffer();

    
};


#endif // BUFFER_H