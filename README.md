**Requirements**

- A C++14 compiler (g++ or clang++), or CMake 3.20 or newer
- The dataset: download PlayerStatistics.csv from https://www.kaggle.com/datasets/eoinamoore/historical-nba-data-and-player-box-scores and place it in the data/ folder. A smaller data/sample.csv is included for quick testing.

**Build**

From the project root, with g++:

    g++ -std=c++14 -O2 -Isrc src/*.cpp -o nba

Or with CMake:

    cmake -S . -B build
    cmake --build build

**Run**

Run from the project root so the program can find the data/ folder:

    ./nba                    (uses data/PlayerStatistics.csv)
    ./nba data/sample.csv    (uses the sample file)

If you built with CMake, the program is build/nba_weighted_rank. On Windows it may be in build/Debug/ instead.

**Using the program**

1. Pick a preset (Pure Scorer, Defensive Legend, All-Around Player), enter custom weights, or quit.
2. Custom weights: enter a number 0 or higher for each stat. Turnovers and fouls lower the score.
3. Team: type a team or city name (for example Lakers or Boston), or * for all teams.
4. Game type: all, regular season only, or playoffs only.
5. Years: all years, or a starting and ending year. Steals and blocks were not recorded before 1974, so use 1974 or later for defensive rankings.
6. The program shows the top 25 performances, the time taken by the Max Heap and by Quickselect, and whether both found the same top scores. It then returns to the menu.