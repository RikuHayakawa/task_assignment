
#include "charging.h"
#include <iostream>

namespace gap
{
    CCharging::CCharging() : m_id(-1), m_time(-1)
    {
    }
    CCharging::CCharging(int id, int time, int m_assignedbinid, int m_assigned_station_id) : m_id(id), m_time(time), m_assignedbinid(m_assignedbinid), m_assigned_station_id(m_assigned_station_id)
    {
    }
    CCharging::~CCharging()
    {
    }
    void 
    CCharging::Print()
    {
        std::cout << "Charging id: " << m_id << " time: " << m_time << " assigned bin id: " << m_assignedbinid << " assigned station id: " << m_assigned_station_id << std::endl;
    }
};
