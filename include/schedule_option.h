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
        bool isDecided = false;
        std::vector<int> binary_option;
        std::vector<std::pair<std::string, int>> m_assignment;

        void setIsDecided(bool status) { isDecided = status; }

    public:
        ScheduleOption();
        ScheduleOption(const std::vector<std::pair<std::string, int>> &assignment, const int robot_id, const bool isDecided, std::vector<int> &binary_option);
        ~ScheduleOption();
    };

}

#endif
