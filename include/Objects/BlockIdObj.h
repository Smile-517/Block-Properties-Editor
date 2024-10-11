#pragma once

class BlockIdObj {
   public:
    // Constructor
    BlockIdObj();

    // Setters
    void setId(int blockIdIn);

    // Getters
    int getBlockId() const;

   protected:
    int blockId;
};