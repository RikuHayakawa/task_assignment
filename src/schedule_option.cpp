#include "schedule_option.h"
#include <iostream>
#include <stdexcept> // std::out_of_range
#include <map>
#include <vector>

namespace schedule_planner
{
    ScheduleOption::ScheduleOption()
    {
    }
    ScheduleOption::ScheduleOption(const std::vector<std::pair<std::string, int>> &assignment, const int robot_id, std::vector<int> &binary_option) : robot_id(robot_id), binary_option(binary_option), m_assignment(assignment)
    {
    }
    ScheduleOption::~ScheduleOption()
    {
    }
};
