/**
 * bin.cpp
 * Author: Zhiyang Su
 * Created on 2014-08-10.
 */

#include "bin.h"
#include <iostream>
#include <stdexcept> // std::out_of_range

namespace gap
{
    CBin::CBin() : m_id(-1), m_size(-1), m_max_size(-1), m_energy_efficiency(-1)
    {
    }

    CBin::CBin(int id, int size, int max_size, int energy_efficiency) 
        : m_id(id), m_size(size), m_max_size(max_size), m_energy_efficiency(energy_efficiency)
    {
    }

    CBin::~CBin()
    {
    }

    // Remove an assignment at specified position
    void CBin::removeAssignment(int pos) {
        if (pos < 0 || pos >= static_cast<int>(m_assignments.size())) {
            throw std::out_of_range("Invalid position for removal.");
        }
        m_assignments.erase(m_assignments.begin() + pos); // Remove the pair at the specified position
    }

    // Add an assignment (pair)
    void CBin::addAssignment(const std::string& name, const int id) {
        m_assignments.emplace_back(name, id); // Using emplace_back to add the pair directly
    }

    // Swap assignments at specified positions
    void CBin::swapAssignments(int startIndex, int endIndex) {
        if (startIndex < 0 || endIndex < 0 || startIndex >= static_cast<int>(m_assignments.size()) || endIndex >= static_cast<int>(m_assignments.size())) {
            throw std::out_of_range("Invalid positions for swap.");
        }

        if (startIndex == endIndex) {
            return;  // No need to swap if the indices are the same
        }

        // Swap elements until reaching the middle of the range
        while (startIndex < endIndex) {
            std::swap(m_assignments[endIndex - 1], m_assignments[endIndex]);
            --endIndex;
        }
    }


    // Display all assignments
    void CBin::displayAssignments() const {
        for (const auto& assignment : m_assignments) {
            std::cout << "Name: " << assignment.first << ", ID: " << assignment.second << std::endl; // Display pair
        }
    }
};
