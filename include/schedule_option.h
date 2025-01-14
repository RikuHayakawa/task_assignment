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
        int m_total_time = 0;
        int m_robot_id = -1;
        std::vector<int> m_binary_option;                      // [0, 0, 0, 1, 1, 1, 1, 0, 0, 0]
        std::vector<std::pair<std::string, int>> m_assignment; // [("task1", 1), ("task2", 2), ("task3", 3)]

    public:
        ScheduleOption();
        ScheduleOption(const std::vector<std::pair<std::string, int>> &assignment, const int robot_id, std::vector<int> &binary_option);
        ~ScheduleOption();
        void removeAssignment(int pos, const int m_item_time);                            // Remove an assignment at specified position
        void addAssignment(const std::string &name, const int id, const int m_item_time); // Add a new assignment (pair), example: ("task", 1)
        void resetAssignment();                                                           // Reset all assignments
        void swapAssignments(int startIndex, int endIndex);                               // Swap two assignments
        void addAssignmentIfNotExists(const std::string &name, int id, std::vector<std::pair<std::string, int>> &before_assignment, std::vector<std::pair<std::string, int>> &assignment);
        void displayAssignments() const; // Display all assignments
    };

}

#endif
