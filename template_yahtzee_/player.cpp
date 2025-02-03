#include "player.h"
#include <iostream>
#include <algorithm>
#include <numeric>

Player::Player() : name("Player"), scoreCard(13, 0) {}

Player::Player(const std::string& name) : name(name), scoreCard(13, 0) {}

const std::string& Player::getName() const {
    return name;
}

// check ken category deja 5tartha walle sinon tzidha score mte3ha
void Player::updateScore(int category, int score) {
    if (!categoriesFilled[category]) {
        scoreCard[category] = score;
        categoriesFilled[category] = true; // Mark the category as filled
    }
}

// display lel options w 9adeh lamit w chnoua mezel
void Player::displayScoreCard() const {
    std::cout << "\nScorecard for " << name << ":\n";
    for (size_t i = 0; i < scoreCard.size(); ++i) {
        std::cout << "Category " << i + 1 << ": " << scoreCard[i] << (categoriesFilled[i] ? "\n" : " (Empty)\n");
    }
}

// systeme score kifeh yet7seb 
int Player::calculateScore(int category, const std::vector<int>& dice) {
    std::vector<int> counts(6, 0);
    for (int die : dice) counts[die - 1]++;

    switch (category) {
    case 0: case 1: case 2: case 3: case 4: case 5:
        return counts[category] * (category + 1); // Upper section: 1s to 6s
    case 6: // Three of a kind
        return *std::max_element(counts.begin(), counts.end()) >= 3 ? std::accumulate(dice.begin(), dice.end(), 0) : 0;
    case 7: // Four of a kind
        return *std::max_element(counts.begin(), counts.end()) >= 4 ? std::accumulate(dice.begin(), dice.end(), 0) : 0;
    case 8: // Full house
        return (std::count(counts.begin(), counts.end(), 3) == 1 && std::count(counts.begin(), counts.end(), 2) == 1) ? 25 : 0;
    case 9: { // Small straight (4 consecutive numbers)
        int consecutive = 0;
        for (int count : counts) {
            if (count > 0) {
                consecutive++;
                if (consecutive >= 4) return 30; // Small straight
            }
            else {
                consecutive = 0;
            }
        }
        return 0;
    }
    case 10: { // Large straight (5 consecutive numbers)
        int consecutive = 0;
        for (int count : counts) {
            if (count > 0) {
                consecutive++;
                if (consecutive >= 5) return 40; // Large straight
            }
            else {
                consecutive = 0;
            }
        }
        return 0;
    }
    case 11: // Yahtzee
        return *std::max_element(counts.begin(), counts.end()) == 5 ? 50 : 0;
    case 12: // Chance
        return std::accumulate(dice.begin(), dice.end(), 0);
    default:
        return 0;
    }
}

int Player::getUpperSectionTotal() const {
    int upperSectionTotal = 0;
    for (int i = 0; i < 6; ++i) { // Categories 1-6 (indices 0-5)
        upperSectionTotal += scoreCard[i];
    }
    return upperSectionTotal;
}



int Player::getTotalScore() const {
    int totalScore = std::accumulate(scoreCard.begin(), scoreCard.end(), 0);
    if (getUpperSectionTotal() >= 63) {
        totalScore += 35;
        std::cout << "35 Bonus points added (UpperSection total score:"<<getUpperSectionTotal() << std::endl; 
    }

    return totalScore;
}

bool Player::isCategoryScored(int category) const {
    return categoriesFilled[category]; // Check if the category has been scored
}