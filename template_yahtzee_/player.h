#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <unordered_map>

class Player {
private:
    std::string name; // Name of the player
    std::vector<int> scoreCard; // Tracks scores for all categories
    bool categoriesFilled[13] = { false }; // Tracks whether categories are filled

public:
    Player(); // Default constructor
    Player(const std::string& name); // Constructor to initialize player with a name
    const std::string& getName() const; // Returns the name of the player
    void updateScore(int category, int score); // Updates the score for a category
    void displayScoreCard() const; // Displays the scorecard of the player
    int calculateScore(int category, const std::vector<int>& dice); // Calculates the score for a category
    int getTotalScore() const; // Returns the total score of the player
    bool isCategoryScored(int category) const; // Checks if a category has been scored
    int getUpperSectionTotal() const; //categories 1-6 for bonus check
};

#endif // PLAYER_H
