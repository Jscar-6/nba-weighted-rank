#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "CSVParser.h"
#include "MaxHeap.h"
#include "QuickSelect.h"

struct Weights {
    double points;
    double assists;
    double rebounds;
    double steals;
    double blocks;
    double turnovers;
    double fouls;
    double eFGP;
};

struct Preset {
    std::string name;
    Weights weights;
};

struct RunResult {
    std::vector<Performance> top;
    double ms;
};

// TODO SHOULD WE add 2 or 3 more presets here, like Defensive and/or "All-Around"??
// Order: points, assists, rebounds, steals, blocks, turnovers, fouls, eFG%
const std::vector<Preset> PRESETS = {
    {"Pure Scorer", {3.0, 0.5, 0.5, 0.5, 0.5, 1.0, 0.0, 1.0}},
};

// eFGP is usually between 0 and 1.5, so it is scaled to matter next to counting stats
const double EFGP_SCALE = 10.0;

double elapsedMs(std::chrono::steady_clock::time_point start,
                 std::chrono::steady_clock::time_point end) {
    return std::chrono::duration<double, std::milli>(end - start).count();
}

// Throws away a bad line of input so the next prompt can read normally
void clearBadInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

double readWeight(const std::string& label) {
    double value;
    while (true) {
        std::cout << "  " << std::left << std::setw(11) << label << ": ";
        if (std::cin >> value && value >= 0) {
            return value;
        }
        if (std::cin.eof()) {
            std::exit(0);
        }
        std::cout << "  Please enter a number that is 0 or higher.\n";
        clearBadInput();
    }
}

Weights readCustomWeights() {
    std::cout << "\nEnter a weight for each stat (0 = ignore it, turnovers and fouls lower the score):\n";
    Weights w;
    w.points = readWeight("Points");
    w.assists = readWeight("Assists");
    w.rebounds = readWeight("Rebounds");
    w.steals = readWeight("Steals");
    w.blocks = readWeight("Blocks");
    w.turnovers = readWeight("Turnovers");
    w.fouls = readWeight("Fouls");
    w.eFGP = readWeight("eFG%");
    return w;
}

int readMenuChoice(int maxChoice) {
    int choice;
    while (true) {
        std::cout << "\nChoose an option: ";
        if (std::cin >> choice && choice >= 1 && choice <= maxChoice) {
            return choice;
        }
        if (std::cin.eof()) {
            std::exit(0);
        }
        std::cout << "Please enter a number from 1 to " << maxChoice << ".\n";
        clearBadInput();
    }
}

void printMenu() {
    std::cout << "\n========== NBA Single-Game Performance Rankings ==========\n";
    int option = 1;
    for (const Preset& preset : PRESETS) {
        std::cout << "  " << option << ". Preset: " << preset.name << "\n";
        option++;
    }
    std::cout << "  " << option << ". Enter custom weights\n";
    option++;
    std::cout << "  " << option << ". Quit\n";
}

void printWeights(const Weights& w) {
    std::cout << "\nWeights  -> PTS " << w.points << " | AST " << w.assists
              << " | REB " << w.rebounds << " | STL " << w.steals
              << " | BLK " << w.blocks << " | TOV " << w.turnovers
              << " | PF " << w.fouls << " | eFG% " << w.eFGP << "\n";
}

void computeScores(std::vector<Performance>& performances, const Weights& w) {
    // TODO 1: set p.score for every performance.
    // Multiply each stat by its weight and add them up, but SUBTRACT turnovers and fouls.
    // Use p.eFGP * EFGP_SCALE * w.eFGP for the eFG% part.
}

RunResult runMaxHeap(std::vector<Performance>& performances) {
    auto start = std::chrono::steady_clock::now();
    MaxHeap heap(performances);  // the constructor makes its own copy
    std::vector<Performance> top = heap.top25();
    auto end = std::chrono::steady_clock::now();
    return {top, elapsedMs(start, end)};
}

RunResult runQuickSelect(const std::vector<Performance>& performances) {
    auto start = std::chrono::steady_clock::now();
    std::vector<Performance> copy = performances;  // copied here so both timers include a copy
    QuickSelect selector;
    std::vector<Performance> top = selector.top25(copy);
    auto end = std::chrono::steady_clock::now();
    return {top, elapsedMs(start, end)};
}

// Compares scores only, because tied performances can come back in a different order
bool scoresMatch(const std::vector<Performance>& a, const std::vector<Performance>& b) {
    // TODO 2: return false if the sizes differ or if any score differ
    return false;
}

void printResults(const std::vector<Performance>& top) {
    std::cout << "\nTop " << top.size() << " performances:\n";
    std::cout << std::left << std::setw(4) << "#" << std::setw(24) << "Player"
              << std::setw(11) << "Date" << std::setw(14) << "Opponent"
              << std::right << std::setw(4) << "PTS" << std::setw(4) << "AST"
              << std::setw(4) << "REB" << std::setw(4) << "STL" << std::setw(4) << "BLK"
              << std::setw(4) << "TOV" << std::setw(4) << "PF" << std::setw(7) << "eFG%"
              << std::setw(9) << "Score" << "\n";
    std::cout << std::string(97, '-') << "\n";

    for (size_t i = 0; i < top.size(); i++) {
        const Performance& p = top[i];
        std::cout << std::left << std::setw(4) << i + 1 << std::setw(24) << p.playerName
                  << std::setw(11) << p.gameDate.substr(0, 10) << std::setw(14) << p.opponent
                  << std::right << std::setw(4) << p.points << std::setw(4) << p.assists
                  << std::setw(4) << p.rebounds << std::setw(4) << p.steals << std::setw(4) << p.blocks
                  << std::setw(4) << p.turnovers << std::setw(4) << p.fouls
                  << std::fixed << std::setprecision(1) << std::setw(7) << p.eFGP * 100
                  << std::setw(9) << p.score << "\n";
    }
    std::cout.unsetf(std::ios::fixed);
    std::cout << std::setprecision(6);
}

void printComparison(const RunResult& heapResult, const RunResult& quickSelectResult) {
    std::cout << "\nExecution time:\n" << std::fixed << std::setprecision(2);
    std::cout << "  Max Heap:    " << heapResult.ms << " ms\n";
    std::cout << "  Quickselect: " << quickSelectResult.ms << " ms\n";

    // TODO print which one was faster and by how many ms, like:
    //   Faster: Quickselect by 245.10 ms

    std::cout.unsetf(std::ios::fixed);
    std::cout << std::setprecision(6);

    if (scoresMatch(heapResult.top, quickSelectResult.top)) {
        std::cout << "  Check: both methods found the same top scores\n";
    } else {
        std::cout << "  Check: WARNING, the top scores do not match\n";
    }
}

void runRanking(std::vector<Performance>& performances, const Weights& weights) {
    printWeights(weights);
    computeScores(performances, weights);

    RunResult heapResult = runMaxHeap(performances);
    RunResult quickSelectResult = runQuickSelect(performances);

    printResults(heapResult.top);
    printComparison(heapResult, quickSelectResult);
}

int main(int argc, char* argv[]) {
    std::string path = argc > 1 ? argv[1] : "data/PlayerStatistics.csv";

    std::cout << "Loading " << path << "...\n";
    auto loadStart = std::chrono::steady_clock::now();
    std::vector<Performance> performances = loadPerformances(path);
    auto loadEnd = std::chrono::steady_clock::now();

    if (performances.empty()) {
        std::cout << "No performances loaded, Check that the file in thjs location" << path << "\n";
        return 1;
    }
    std::cout << "Loaded " << performances.size() << " performances in "
              << elapsedMs(loadStart, loadEnd) / 1000.0 << " seconds.\n";

    const int customOption = static_cast<int>(PRESETS.size()) + 1;
    const int quitOption = customOption + 1;

    while (true) {
        printMenu();
        int choice = readMenuChoice(quitOption);

        if (choice == quitOption) {
            break;
        }
        if (choice == customOption) {
            runRanking(performances, readCustomWeights());
        } else {
            runRanking(performances, PRESETS[choice - 1].weights);
        }
    }

    std::cout << "Thanks for using!\n";
    return 0;
}
