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
#include <algorithm>
#include <cctype>

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
    {"Defensive Legend", {1.0, 1.0, 1.0, 5.0, 5.0, 1.0, 2.0, 2.0}},
    {"All-Around Player", {0.8, 1.5, 1.5, 1.0, 1.0, 1.5, 2.0, 2.0}}
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
    for (Performance& performance : performances) {
        performance.score = performance.points * w.points + performance.assists * w.assists +
            performance.rebounds * w.rebounds + performance.steals * w.steals + performance.blocks * w.blocks -
                performance.turnovers * w.turnovers - performance.fouls * w.fouls + performance.eFGP *
                    EFGP_SCALE * w.eFGP;
    }
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
    if (a.size() != b.size()) {
        return false;
    }
    for (int i = 0; i < static_cast<int>(a.size()); i++) {
        if (a[i].score != b[i].score) {
            return false;
        }
    }
    return true;
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

    if (quickSelectResult.ms < heapResult.ms) {
        std::cout << "  Faster: Quickselect by " << heapResult.ms - quickSelectResult.ms << " ms\n";
    }
    else if (quickSelectResult.ms > heapResult.ms) {
        std::cout << "  Faster: Max Heap by " << quickSelectResult.ms - heapResult.ms << " ms\n";
    }
    else {
        std::cout << "  Faster: Both took the same amount of time\n";
    }

    std::cout.unsetf(std::ios::fixed);
    std::cout << std::setprecision(6);

    if (scoresMatch(heapResult.top, quickSelectResult.top)) {
        std::cout << "  Check: both methods found the same top scores\n";
    } else {
        std::cout << "  Check: WARNING, the top scores do not match\n";
    }
}

std::string lowercase(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(),
                   [](unsigned char c) {
                       return static_cast<char>(std::tolower(c));
                   });
    return text;
}

std::vector<Performance> filterByTeam(
    const std::vector<Performance>& performances) {

    std::cout << "\nTeam name (example: Lakers), or * for all teams: ";
    std::string team;
    std::getline(std::cin >> std::ws, team);

    if (team == "*") {
        return performances;
    }

    team = lowercase(team);
    std::vector<Performance> filtered;

    for (const auto& performance : performances) {
        if ((" " + lowercase(performance.team)).find(" " + team) != std::string::npos) {
            filtered.push_back(performance);
        }
    }

    return filtered;
}

std::vector<Performance> filterByGameType(
    const std::vector<Performance>& performances) {

    std::cout << "\nGame type:\n"
              << "1. All loaded game types\n"
              << "2. Regular season only\n"
              << "3. Playoffs only\n";

    int choice = readMenuChoice(3);

    if (choice == 1) {
        return performances;
    }

    const std::string wanted =
        choice == 2 ? "Regular Season" : "Playoffs";

    std::vector<Performance> filtered;

    for (const auto& performance : performances) {
        if (performance.gameType == wanted) {
            filtered.push_back(performance);
        }
    }

    return filtered;
}

int readYear(const std::string& prompt, int minimum) {
    while (true) {
        std::cout << prompt;
        std::string input;

        if (!(std::cin >> input)) {
            std::exit(0);
        }

        if (input.size() == 4 &&
            input.find_first_not_of("0123456789") == std::string::npos) {
            int year = std::stoi(input);
            if (year >= minimum) {
                return year;
            }
            }

        std::cout << "Enter a four-digit year from "
                  << minimum << " to 9999.\n";
    }
}

std::vector<Performance> filterByYear(
    const std::vector<Performance>& performances) {

    std::cout << "\nYear filter:\n"
              << "1. All years\n"
              << "2. Choose a calendar-year range\n";

    if (readMenuChoice(2) == 1) {
        return performances;
    }

    int firstYear = readYear("Starting year: ", 1000);
    int lastYear = readYear("Ending year: ", firstYear);

    std::vector<Performance> filtered;

    for (const auto& performance : performances) {
        if (performance.gameDate.size() < 4) {
            continue;
        }

        std::string yearText = performance.gameDate.substr(0, 4);
        if (yearText.find_first_not_of("0123456789")
            != std::string::npos) {
            continue;
            }

        int year = std::stoi(yearText);
        if (year >= firstYear && year <= lastYear) {
            filtered.push_back(performance);
        }
    }

    return filtered;
}

void runRanking(std::vector<Performance>& performances,
                const Weights& weights) {
    std::vector<Performance> filtered = filterByTeam(performances);
    filtered = filterByGameType(filtered);
    filtered = filterByYear(filtered);

    if (filtered.empty()) {
        std::cout << "\nNo performances match those filters. Returning to menu.\n";
        return;
    }

    std::cout << "\nRanking " << filtered.size()
              << " matching performances.\n";

    printWeights(weights);
    computeScores(filtered, weights);

    RunResult heapResult = runMaxHeap(filtered);
    RunResult quickSelectResult = runQuickSelect(filtered);

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
