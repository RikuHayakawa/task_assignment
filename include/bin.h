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
        int m_constraint_time;
        int m_charge_time_for_guarantee;
        schedule_planner::ScheduleOption m_initial_assignment;
        schedule_planner::ScheduleOption m_scheduled_assignment;

        CBin();
        CBin(int id, int size, int max_size,
             int initial_size, int constraint_time, int charge_time_for_guaranteed_energy);
        virtual ~CBin();

        void Print();
    };
};

#endif
