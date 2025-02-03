#include "game.h"
#include <iostream>
#include <fstream>   // For file handling
#include <sstream>   // For string handling
#include <iomanip>   // For std::setw
#include <limits>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <algorithm>

// Constructor
Game::Game() : rounds(13) {
    // Players will be initialized in initializeGame
}

void Game::displayHighScores() {
    std::ifstream highScoreFile("highscores.txt");
    if (!highScoreFile) {
        std::cout << "No high scores available yet.\n";
        return;
    }

    std::cout << "\n--- High Scores ---\n";
    std::string playerName;
    int score;

    while (highScoreFile >> playerName >> score) {
        std::cout << std::setw(15) << std::left << playerName << score << "\n";
    }
    highScoreFile.close();
    std::cout << "-------------------\n";
}

void Game::updateHighScores() {
    std::ifstream highScoreFile("highscores.txt");
    std::unordered_map<std::string, int> highScores;

    // Read existing highscores
    std::string playerName;
    int score;
    while (highScoreFile >> playerName >> score) {
        highScores[playerName] = score;
    }
    highScoreFile.close();

    // Update highscores with current game results
    for (const auto& player : players) {
        int totalScore = player.getTotalScore();
        if (highScores.find(player.getName()) == highScores.end() || totalScore > highScores[player.getName()]) {
            highScores[player.getName()] = totalScore;
        }
    }

    // Write updated highscores back to the file
    std::ofstream outFile("highscores.txt");
    for (const auto& entry : highScores) {
        outFile << entry.first << " " << entry.second << "\n";
    }
    outFile.close();
}


// Check and validate numeric input in range
int Game::getValidatedInput(int min, int max) {
    int input;
    while (true) {
        std::cin >> input;
        if (std::cin.fail() || input < min || input > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please try again: ";
        }
        else {
            return input;
        }
    }
}

// Display game rules
void Game::displayRules() {
    std::cout << "Rules of Yahtzee:\n";
    std::cout << "1. Roll 5 dice to achieve specific combinations.\n";
    std::cout << "2. You have a maximum of 3 rolls per turn.\n";
    std::cout << "3. Choose a category to score your points.\n";
    std::cout << "4. The player with the highest score after 13 rounds wins!\n";
    std::cout << "5. Bonus points are awarded for specific combinations.\n";
}

// starting player
int Game::determineStartingPlayer() {
    std::vector<int> rolls;
    for (size_t i = 0; i < players.size(); ++i) {
        dice.rollAll();
        int rollTotal = std::accumulate(dice.getDice().begin(), dice.getDice().end(), 0);
        rolls.push_back(rollTotal);
        std::cout << players[i].getName() << " rolled: ";
        for (int die : dice.getDice()) {
            std::cout << die << " ";
        }
        std::cout << " (Total: " << rollTotal << ")\n";
    }

    auto maxRoll = std::max_element(rolls.begin(), rolls.end());
    int startingPlayerIndex = std::distance(rolls.begin(), maxRoll);
    std::cout << players[startingPlayerIndex].getName() << " will start the game!\n";
    return startingPlayerIndex;
}


void Game::initializeGame() {
    std::cout << "Initializing game...\n";
    std::cout << "Enter the number of players: ";
    int numPlayers = getValidatedInput(1, 4);

    players.clear(); // Clear any previous players
    for (int i = 0; i < numPlayers; ++i) {
        std::string name;
        std::cout << "Enter name for player " << i + 1 << ": ";
        std::cin >> name;
        players.emplace_back(name); // Add players to the vector
    }

    // Determine starting player
    int startingPlayerIndex = determineStartingPlayer();
    // Rotate the players vector so the starting player is first
    std::rotate(players.begin(), players.begin() + startingPlayerIndex, players.end());

    std::cout << "Players initialized. Starting game...\n";
}
// Game loop
void Game::playRound() {
    for (int round = 0; round < rounds; ++round) {
        std::cout << "\n--- Round " << round + 1 << " ---\n";

        for (auto& player : players) {
            std::cout << "It's " << player.getName() << "'s turn.\n";

            // Initial roll
            dice.rollAll();
            std::cout << "Initial roll: ";
            for (int die : dice.getDice()) {
                std::cout << die << " ";
            }
            std::cout << "\n";

            // Roll up to 2 more times
            for (int roll = 1; roll < 3; ++roll) {
                std::cout << "Would you like to roll again? (y/n): ";
                char choice;
                std::cin >> choice;

                if (choice == 'n' || choice == 'N') break;

                std::cout << "Enter dice indices to reroll (space-separated, e.g., 0 1 4): ";
                std::cin.ignore(); // Clear newline character
                std::string inputLine;
                std::getline(std::cin, inputLine);

                std::vector<int> indices;
                std::istringstream iss(inputLine);
                int index;
                while (iss >> index) {
                    if (index >= 0 && index < 5) {
                        indices.push_back(index);
                    }
                    else {
                        std::cout << "Invalid index: " << index << ". Please enter indices between 0 and 4.\n";
                    }
                }

                dice.rollSelected(indices);

                std::cout << "Current dice: ";
                for (int die : dice.getDice()) {
                    std::cout << die << " ";
                }
                std::cout << "\n";
            }

            // Display categories and prompt for a selection
            std::cout << "\nSelect a category to score:\n";
            std::vector<std::string> categories = {
                "1. Ones        - Sum of all dice showing 1",
                "2. Twos        - Sum of all dice showing 2",
                "3. Threes      - Sum of all dice showing 3",
                "4. Fours       - Sum of all dice showing 4",
                "5. Fives       - Sum of all dice showing 5",
                "6. Sixes       - Sum of all dice showing 6",
                "7. Three of a Kind - Sum of all dice if 3 are the same",
                "8. Four of a Kind  - Sum of all dice if 4 are the same",
                "9. Full House     - 25 points for 3 of a kind + 2 of a kind",
                "10. Small Straight - 30 points for 4 consecutive dice",
                "11. Large Straight - 40 points for 5 consecutive dice",
                "12. Yahtzee       - 50 points for all 5 dice the same",
                "13. Chance        - Sum of all dice"
            };

            for (int i = 0; i < categories.size(); ++i) {
                std::cout << categories[i];
                if (player.isCategoryScored(i)) {
                    std::cout << " [Already scored]";
                }
                else {
                    int potentialPoints = player.calculateScore(i, dice.getDice());
                    std::cout << " [Empty, Potential: " << potentialPoints << " points]";
                }
                std::cout << "\n";
            }

            std::cout << "Enter the number of the category you want to score: ";
            int task = getValidatedInput(1, 13) - 1;

            if (player.isCategoryScored(task)) {
                std::cout << "This category is already scored. Please choose another.\n";
                task = getValidatedInput(1, 13) - 1;
            }

            // Update score
            int score = player.calculateScore(task, dice.getDice());
            player.updateScore(task, score);

            std::cout << "Score updated! Current scorecard:\n";
            player.displayScoreCard();
        }
    }

    // Display final scores for all players
    for (const auto& player : players) {
        std::cout << "\n" << player.getName() << "'s Total Score: " << player.getTotalScore() << "\n";
    }
    updateHighScores();
    displayMenu();
}


void Game::displayMenu() {
    while (true) {
        std::cout << "1. Start Game\n";
        std::cout << "2. Rules\n";
        std::cout << "3. High Scores\n";
        std::cout << "4. Exit\n";
        std::cout << "Choose an option: ";

        int choice = getValidatedInput(1, 4);

        switch (choice) {
        case 1:
            initializeGame();
            playRound();
            return;
        case 2:
            displayRules();
            break;
        case 3:
            displayHighScores();
            break;
        case 4:
            std::cout << "Exiting game. Goodbye!\n";
            return;
        }

        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }
}
