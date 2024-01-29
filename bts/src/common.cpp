#include "common.h"

bool are_all_elements_minus_one(const std::vector<double>& vec) {
    for (double element : vec) {
        if (element != -1.) {
            return false;
        }
    }
    return true;
}

bool are_all_elements_zero(const std::vector<double>& vec) {
    for (double element : vec) {
        if (element != 0) {
            return false;
        }
    }
    return true;
}
