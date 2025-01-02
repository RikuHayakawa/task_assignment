#ifndef CCHARGING_SCHEDULE_H
#define CCHARGING_SCHEDULE_H
#include <map>
#include <vector>
#include <string>

namespace schedule_option
{
    class ScheduleOption
    {
    public:
        int id;
        int robot_id;
        bool isDecided = false;
        std::vector<std::vector<int>> binary_option;
        std::vector<std::pair<std::string, int>> m_assignment;

        void setIsDecided(bool status) { isDecided = status; }
        static ScheduleOption createScheduleOptions(const std::vector<int> &taskIds, const int robot_id, const int constantTime);
    };

}

#endif
