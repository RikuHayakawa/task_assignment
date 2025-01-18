#ifndef SCHEDULE_OPTION_H
#define SCHEDULE_OPTION_H
#include <map>
#include <vector>
#include <string>
#include <charging.h>

namespace schedule_planner
{
    class ScheduleOption
    {
    public:
        int m_robot_id;
        int m_rest_energy;
        int m_total_time = 0;
        int m_constraint_time;
        int max_size;
        std::vector<int> m_binary_option;                      // [0, 0, 0, 1, 1, 1, 1, 0, 0, 0]
        std::vector<std::pair<std::string, int>> m_assignment; // [("task1", 1), ("task2", 2), ("task3", 3)]

    public:
        ScheduleOption(const int robot_id, const int initial_energy, const int constraint_time, const int max_size);
        ~ScheduleOption();
        void addAssignment(const std::string &name, const int id, const int m_item_time, const int m_item_energy); // Add an assignment (pair)
        void removeChargeAssignment();
        void swapAssignments(int startIndex, int endIndex); // Swap two assignments
        void displayAssignments() const;                    // Display all assignments
    };

}

#endif
