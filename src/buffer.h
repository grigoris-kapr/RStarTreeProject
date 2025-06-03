#ifndef BUFFER_H
#define BUFFER_H

class Buffer {
public:
    Buffer(int dataSize, int treeSize);
    ~Buffer();

    TreeNode* getTreeNode(int blockID);
    DataBlock* getDataBlock(int blockID);
    



#endif // BUFFER_H