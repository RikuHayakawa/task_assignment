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
    CBin::CBin() : m_id(-1), m_size(-1), m_max_size(-1), m_initial_size(-1), m_constraint_time(-1), m_initial_assignment(-1, -1, -1, -1), m_scheduled_assignment(-1, -1, -1, -1)
    {
    }

    CBin::CBin(int id, int size, int max_size, int initial_size, int constraint_time)
        : m_id(id), m_size(size), m_max_size(max_size), m_initial_size(initial_size), m_initial_assignment(id, initial_size, constraint_time, max_size), m_scheduled_assignment(id, initial_size, constraint_time, max_size)
    {
    }

    CBin::~CBin()
    {
    }

    void CBin::Print()
    {
        std::cout << "Bin id: " << m_id << ", size: " << m_size << ", max size: " << m_max_size << ", initial size: " << m_initial_size << std::endl;
        std::cout << "Initial assignment: " << std::endl;
        m_initial_assignment.displayAssignments();
        std::cout << "Scheduled assignment: " << std::endl;
        m_scheduled_assignment.displayAssignments();
    }
};
