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
        int m_energy_efficiency;
        int m_total_time = 0;
        int m_assigned_chargeing_id = -1;
        std::vector<std::pair<std::string, int>>
            m_assignment; // To hold pairs of (name, id)
        schedule_planner::ScheduleOption m_scheduled_assignment;

        CBin();
        CBin(int id, int size, int max_size,
             int initial_size,
             int energy_efficiency);
        virtual ~CBin();

        void removeAssignment(int pos, const int m_item_time);                            // Remove an assignment at specified position
        void addAssignment(const std::string &name, const int id, const int m_item_time); // Add a new assignment (pair), example: ("task", 1)
        void resetAssignment();                                                           // Reset all assignments
        void swapAssignments(int startIndex, int endIndex);                               // Swap two assignments
        void displayAssignments() const;                                                  // Display all assignments
    };
};

#endif
