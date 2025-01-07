#ifndef SCHEDULE_OPTION_H
#define SCHEDULE_OPTION_H
#include <map>
#include <vector>
#include <string>

namespace schedule_planner
{
    class ScheduleOption
    {
    public:
        int robot_id;
        std::vector<int> binary_option;                        // [0, 0, 0, 1, 1, 1, 1, 0, 0, 0]
        std::vector<std::pair<std::string, int>> m_assignment; // [("task1", 1), ("task2", 2), ("task3", 3)]

    public:
        ScheduleOption();
        ScheduleOption(const std::vector<std::pair<std::string, int>> &assignment, const int robot_id, std::vector<int> &binary_option);
        ~ScheduleOption();
    };

}

#endif
