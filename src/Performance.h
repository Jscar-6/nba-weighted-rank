#pragma once
#include <string>

struct Performance {
    std::string playerName;
    std::string gameDate;
    std::string team;
    std::string opponent;
    std::string gameType;

    int points = 0;
    int assists = 0;
    int rebounds = 0;
    int steals = 0;
    int blocks = 0;
    int turnovers = 0;
    int fouls = 0;

    double eFGP = 0.0;
    double score = 0.0;
};