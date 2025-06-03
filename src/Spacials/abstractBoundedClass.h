#ifndef ABSTRACTBOUNDEDCLASS_H
#define ABSTRACTBOUNDEDCLASS_H

#include <vector>

class AbstractBoundedClass {
public:
    AbstractBoundedClass() = default;
    virtual ~AbstractBoundedClass() = default;

    virtual const std::vector<double>& getStart() const = 0;
    virtual const std::vector<double>& getEnd() const = 0;
};
#endif // ABSTRACTBOUNDEDCLASS_H