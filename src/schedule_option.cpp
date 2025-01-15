#include "schedule_option.h"
#include <iostream>
#include <stdexcept> // std::out_of_range
#include <map>
#include <vector>
#include <algorithm>

namespace schedule_planner
{
    ScheduleOption::ScheduleOption() : m_robot_id(-1), m_binary_option(std::vector<int>()), m_assignment(std::vector<std::pair<std::string, int>>())
    {
    }
    ScheduleOption::ScheduleOption(const std::vector<std::pair<std::string, int>> &assignment, const int robot_id) : m_robot_id(robot_id), m_assignment(assignment)
    {
    }
    ScheduleOption::~ScheduleOption()
    {
    }
    // Add an assignment (pair)
    void ScheduleOption::addAssignment(const std::string &name, const int id)
    {
        m_assignment.emplace_back(name, id); // Using emplace_back to add the pair directly
    }

    void ScheduleOption::resetAssignment()
    {
        m_assignment.clear();
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
    }

    void ScheduleOption::addAssignmentIfNotExists(const std::string &name, int id, std::vector<std::pair<std::string, int>> &before_assignment, std::vector<std::pair<std::string, int>> &assignment)
    {
        // m_assignment内の各要素をチェック
        auto it = std::find_if(before_assignment.begin(), before_assignment.end(), [id](const std::pair<std::string, int> &before_assignment)
                               { return before_assignment.second == id; });

        // 一致する要素がない場合に新しい要素を追加
        if (it == before_assignment.end())
        {
            assignment.push_back(std::make_pair(name, id));
        }
    }
};
