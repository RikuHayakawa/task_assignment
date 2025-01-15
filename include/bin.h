#ifndef BIN_H
#define BIN_H

#include <vector>
#include <string>
#include <utility> // std::pair
#include "schedule_option.h"

namespace gap
{
    class CBin
    {
    public:
        int m_id;
        int m_size;
        int m_max_size;
        int m_initial_size;
        int m_rest_energy;
        int m_total_time = 0;
        schedule_planner::ScheduleOption m_initial_assignment;
        schedule_planner::ScheduleOption m_scheduled_assignment;

        CBin();
        CBin(int id, int size, int max_size,
             int initial_size);
        virtual ~CBin();

        void addAssignmentAndUpdateBin(const std::string &name, const int id, const int m_item_time, const int m_item_energy); // Add an assignment (pair)
        void Print();
    };
};

#endif
