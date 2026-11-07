#include "QuickSelect.h"
#include <algorithm>
#include <random>
#include <utility>

int QuickSelect::divide(std::vector<Performance>& data,
                        int start, int end, int targetIndex) {
    double pivotScore = data[targetIndex].score;
    std::swap(data[targetIndex], data[end]);

    int position = start;

    for (int i = start; i < end; i++) {
        if (data[i].score > pivotScore) {
            std::swap(data[i], data[position]);
            position++;
        }
    }

    std::swap(data[position], data[end]);
    return position;
}

void QuickSelect::quickSelect(std::vector<Performance>& data,
                              int start, int end, int targetIndex) {
    static std::mt19937 generator(std::random_device{}());

    while (start < end) {
        std::uniform_int_distribution<int> distribution(start, end);
        int pivotIndex = distribution(generator);

        int position = divide(data, start, end, pivotIndex);

        if (position == targetIndex) {
            return;
        }

        if (position > targetIndex) {
            end = position - 1;
        } else {
            start = position + 1;
        }
    }
}

std::vector<Performance> QuickSelect::top25(
    std::vector<Performance>& data) {
    if (data.empty()) {
        return {};
    }

    int count = data.size() < 25
        ? static_cast<int>(data.size())
        : 25;

    if (data.size() > 25) {
        quickSelect(data, 0, static_cast<int>(data.size()) - 1,
                    count - 1);
    }

    std::vector<Performance> results(
        data.begin(), data.begin() + count);

    std::sort(results.begin(), results.end(),
              [](const Performance& a, const Performance& b) {
                  return a.score > b.score;
              });

    return results;
}