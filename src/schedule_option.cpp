#include "schedule_option.h"
#include <iostream>
#include <stdexcept> // std::out_of_range
#include <map>
#include <vector>
#include <algorithm>

namespace schedule_planner
{
    ScheduleOption::ScheduleOption(const int robot_id, const int initial_energy, const int constraint_time, const int max_size)
        : m_robot_id(robot_id), m_rest_energy(initial_energy), m_constraint_time(constraint_time), max_size(max_size)
    {
    }
    ScheduleOption::~ScheduleOption()
    {
    }
    // Add an assignment (pair)
    void ScheduleOption::addAssignment(const std::string &name, const int id, const int m_item_time, const int m_item_energy)
    {
        const int item_energy = (name == "charging") ? -m_item_energy : m_item_energy;
        m_total_time += m_item_time;
        m_rest_energy -= item_energy;
        if (m_rest_energy < 0)
        {
            std::cerr << "Error: Rest energy is less than 0. DP cannot proceed." << std::endl;
            exit(1);
        }
        if (m_total_time > m_constraint_time)
        {
            displayAssignments();
            std::cerr << "Error: Constraint time (" << m_constraint_time
                      << ") is less than or equal to the total assigned time ("
                      << m_total_time << "). DP cannot proceed." << std::endl;
            exit(1);
        }
        if (m_rest_energy > max_size)
        {
            displayAssignments();
            std::cerr << "Error: Rest energy is more than max size. DP cannot proceed." << std::endl;
            exit(1);
        }
        m_assignment.push_back(std::make_pair(name, id));
    }

    void ScheduleOption::removeChargeAssignment()
    {
        // Remove all charging assignments
        m_assignment.erase(
            std::remove_if(m_assignment.begin(), m_assignment.end(),
                           [](const std::pair<std::string, int> &assignment)
                           {
                               return assignment.first == "charging";
                           }),
            m_assignment.end());
    }

    // Swap assignments at specified positions
    void ScheduleOption::swapAssignments(int startIndex, int endIndex)
    {
        if (startIndex < 0 || endIndex < 0 || startIndex >= static_cast<int>(m_assignment.size()) || endIndex >= static_cast<int>(m_assignment.size()))
        {
            throw std::out_of_range("Invalid positions for swap.");
        }

        if (startIndex == endIndex)
        {
            return; // No need to swap if the indices are the same
        }

        // Swap elements until reaching the middle of the range
        while (startIndex < endIndex)
        {
            std::swap(m_assignment[endIndex - 1], m_assignment[endIndex]);
            --endIndex;
        }
    }

    // Display all assignments
    void ScheduleOption::displayAssignments() const
    {
        for (const auto &assignment : m_assignment)
        {
            std::cout << "Name: " << assignment.first << ", ID: " << assignment.second << std::endl; // Display pair
        }
        // binary_optionを表示
        std::cout << "Binary option: ";
        for (const auto &option : m_binary_option)
        {
            std::cout << option << " ";
        }
        std::cout << std::endl;
        std::cout << "Rest energy: " << m_rest_energy << std::endl;
        std::cout << "Total time: " << m_total_time << std::endl;
    }

};
