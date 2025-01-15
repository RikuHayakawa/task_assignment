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
    CBin::CBin() : m_id(-1), m_size(-1), m_max_size(-1)
    {
    }

    CBin::CBin(int id, int size, int max_size, int initial_size)
        : m_id(id), m_size(size), m_max_size(max_size), m_initial_size(initial_size), m_rest_energy(initial_size)
    {
    }

    CBin::~CBin()
    {
    }

    // Add an assignment (pair)
    void CBin::addAssignmentAndUpdateBin(const std::string &name, const int id, const int m_item_time, const int m_item_energy)
    {
        const int item_energy = (name == "charging") ? -m_item_energy : m_item_energy;
        m_total_time += m_item_time;
        m_rest_energy -= item_energy;
        m_initial_assignment.addAssignment(name, id); // Using emplace_back to add the pair directly
    }

    void CBin::Print()
    {
        std::cout << "Bin id: " << m_id << ", size: " << m_size << ", max size: " << m_max_size << ", initial size: " << m_initial_size << ", Rest energy: " << m_rest_energy << std::endl;
        std::cout << "Initial assignment: " << std::endl;
        m_initial_assignment.displayAssignments();
        std::cout << "Scheduled assignment: " << std::endl;
        m_scheduled_assignment.displayAssignments();
    }
};
