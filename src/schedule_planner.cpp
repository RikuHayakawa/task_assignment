#include "schedule_planner.h"
#include <iostream>
#include <stdexcept> // std::out_of_range
#include <map>
#include <vector>

namespace schedule_planner
{
    SchedulePlanner::SchedulePlanner() : m_id(-1), m_time(-1)
    {
    }
    SchedulePlanner::SchedulePlanner(int id, int time, int m_assignedbinid, int m_assigned_station_id) : m_id(id), m_time(time), m_assignedbinid(m_assignedbinid), m_assigned_station_id(m_assigned_station_id)
    {
    }
    SchedulePlanner::~SchedulePlanner()
    {
    }

};
