#include "CSVParser.h"
#include <fstream>
#include <iostream>

const int COL_FIRST_NAME = 0;
const int COL_LAST_NAME = 1;
const int COL_DATE = 4;
const int COL_TEAM_CITY = 5;
const int COL_TEAM_NAME = 6;
const int COL_OPPONENT =  8;
const int COL_GAME_TYPE = 9;
const int COL_POINTS = 16;
const int COL_ASSISTS = 17;
const int COL_BLOCKS = 18;
const int COL_STEALS = 19;
const int COL_FOULS = 32;
const int COL_TURNOVERS = 33;
const int COL_REBOUNDS = 31;
const int COL_FGA = 20;
const int COL_FGM = 21;
const int COL_TPM = 24; // three pointers made

//splits one csv line into its fields (string form, so all stored in one string vector>
std::vector<std::string> splitLine(const std::string& entryLine){
  std::vector<std::string> sections;
  std::string curr;
  bool inQ = false;
    for (char c : entryLine){
        if (c == '"'){
            inQ = !inQ; //entering or leaving quotes sections 
        }
        else if (c == ',' && !inQ){
            sections.push_back(curr);
            curr.clear(); 
        }
        else {
            curr += c; //normal cahrachter
        }
    }
    sections.push_back(curr);
    return sections;
}




std::vector<Performance> loadPerformances(const std::string& path){
    std::vector<Performance> performances;

    std::ifstream file(path);
    if (!file.is_open()){
        std::cout << "couldnt open" << std::endl;
        return performances;
    }

    std::string line;
    std::getline(file, line);

    while(std::getline(file, line)){
        std::vector<std::string> fields = splitLine(line);
       
        if (fields.size() <= COL_TURNOVERS) {
            continue;
        }

        if (fields[COL_POINTS].find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        const std::string& type = fields[COL_GAME_TYPE];
        if (type != "Regular Season" && type != "Playoffs" && type != "Play-in Tournament"){
            continue;
        } 
        Performance perf;
        perf.playerName = fields[COL_FIRST_NAME] + " " + fields[COL_LAST_NAME];
        perf.gameDate = fields[COL_DATE];
        perf.team = fields[COL_TEAM_CITY] + " " + fields[COL_TEAM_NAME];
        perf.gameType = fields[COL_GAME_TYPE];
        perf.points = static_cast<int>(std::stod(fields[COL_POINTS]));
        perf.opponent = fields[COL_OPPONENT];
        perf.assists = static_cast<int>(std::stod(fields[COL_ASSISTS]));
        perf.rebounds = static_cast<int>(std::stod(fields[COL_REBOUNDS]));
        perf.steals = static_cast<int>(std::stod(fields[COL_STEALS]));
        perf.blocks = static_cast<int>(std::stod(fields[COL_BLOCKS]));
        perf.turnovers = static_cast<int>(std::stod(fields[COL_TURNOVERS]));
        perf.fouls = static_cast<int>(std::stod(fields[COL_FOULS]));
        const double shotsMade = std::stod(fields[COL_FGM]);
        const double threesMade = std::stod(fields[COL_TPM]);
        const double shotsAttempted = std::stod(fields[COL_FGA]);
        // A few 1950s-60s rows have more makes than attempts, so eFGP stays 0 for those
        if (shotsAttempted > 0 && shotsMade <= shotsAttempted) {
            perf.eFGP = (shotsMade + 0.5 * threesMade) / shotsAttempted;
        }
        performances.push_back(perf);
    }
    return performances;
}