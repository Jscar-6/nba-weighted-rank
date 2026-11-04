//
// Created by thoma on 11/3/2026.
//

#ifndef NBA_WEIGHTED_RANK_QUICKSELECT_H
#define NBA_WEIGHTED_RANK_QUICKSELECT_H
#include <vector>

#include "Performance.h"


class QuickSelect {
private:
    int divide(std::vector<Performance>& data, int start, int end, int targetIndex);
    void quickSelect(std::vector<Performance>& data, int start, int end, int targetIndex);
public:
    std::vector<Performance> top25(std::vector<Performance>& data);
};


#endif //NBA_WEIGHTED_RANK_QUICKSELECT_H
