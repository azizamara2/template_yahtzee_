#include "dice.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Dice::Dice() : dice(5, 1) {
    std::srand(std::time(nullptr));
}

void Dice::rollAll() {
    for (int& die : dice) {
        die = std::rand() % 6 + 1;
    }
}

void Dice::rollSelected(const std::vector<int>& indices) {
    for (int index : indices) {
        dice[index] = std::rand() % 6 + 1;
    }
}

const std::vector<int>& Dice::getDice() const {
    return dice;
}