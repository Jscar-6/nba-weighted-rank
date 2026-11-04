#include <iostream>
#include <vector>
#include "QuickSelect.h"
#include <chrono>

int main() {
    std::vector<Performance> data;


    for (int i = 1; i <= 30; i++) {
        Performance game;
        game.playerName = "Player " + std::to_string(i);
        game.score = i;
        data.push_back(game);
    }

    QuickSelect selector;
    auto start = std::chrono::steady_clock::now();

    std::vector<Performance> results = selector.top25(data);

    auto end = std::chrono::steady_clock::now();

    double milliseconds =
        std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "Quickselect time: "
              << milliseconds << " ms\n";
    bool passed = results.size() == 25;

    for (int i = 0; i < static_cast<int>(results.size()); i++) {
        std::cout << results[i].playerName
                  << " | Score: " << results[i].score << '\n';

        if (results[i].score != 30 - i) {
            passed = false;
        }
    }

    std::cout << (passed ? "\nTEST PASSED\n" : "\nTEST FAILED\n");


    std::vector<Performance> empty;
    bool emptyPassed = selector.top25(empty).empty();


    std::vector<Performance> small(3);
    small[0].score = -5;
    small[1].score = 10;
    small[2].score = 0;

    auto smallResults = selector.top25(small);
    bool smallPassed =
        smallResults.size() == 3 &&
        smallResults[0].score == 10 &&
        smallResults[1].score == 0 &&
        smallResults[2].score == -5;


    std::vector<Performance> ties(40);
    for (auto& game : ties) {
        game.score = 7;
    }

    auto tieResults = selector.top25(ties);
    bool tiesPassed = tieResults.size() == 25;
    for (const auto& game : tieResults) {
        if (game.score != 7) {
            tiesPassed = false;
        }
    }

    std::cout << "Empty input: " << (emptyPassed ? "PASS" : "FAIL") << '\n';
    std::cout << "Small input: " << (smallPassed ? "PASS" : "FAIL") << '\n';
    std::cout << "Tied scores: " << (tiesPassed ? "PASS" : "FAIL") << '\n';

    passed = passed && emptyPassed && smallPassed && tiesPassed;

    return passed ? 0 : 1;
}