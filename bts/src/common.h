#include <vector>

#pragma once

#define MANIPULATION  // Comment to not connect to grasping interface
#define GAZE  // Comment to not use gaze controller
#define EMOTIONS

#define ICUB
// #define ECUB

bool are_all_elements_minus_one(const std::vector<double>& vec);

bool are_all_elements_zero(const std::vector<double>& vec);

bool are_all_elements_minus_two(const std::vector<double>& vec);
