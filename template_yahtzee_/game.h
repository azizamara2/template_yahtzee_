// Game.h
#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Dice.h"
#include <vector>

class Game {
private:
    std::vector<Player> players; // Add this for handling multiple players
    Dice dice; // Represents the dice used in the game
    const int rounds; // Total number of rounds in the game

    int getValidatedInput(int min, int max); // Gets and validates user input
    void displayRules(); // Displays the rules of the game
    void initializeGame(); // Initializes the game settings
    void playRound(); // Handles the logic for playing a single round
    void displayHighScores(); // Displays high scores


public:
    Game(); // Constructor to initialize the game
    void displayMenu(); // Displays the main menu of the game
};

#endif // GAME_H