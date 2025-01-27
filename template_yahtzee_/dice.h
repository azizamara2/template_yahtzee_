#ifndef DICE_H
#define DICE_H

#include <vector>

class Dice {
private:
    std::vector<int> dice; // Stores the current values of the dice

public:
    Dice(); // Constructor to initialize the dice
    void rollAll(); // Rolls all dice
    void rollSelected(const std::vector<int>& indices); // Rolls selected dice by indices
    const std::vector<int>& getDice() const; // Returns the current values of the dice
};

#endif // DICE_H