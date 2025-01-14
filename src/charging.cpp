
#include "charging.h"
#include <iostream>

namespace gap
{
    CCharging::CCharging() : m_id(-1), m_time(-1), m_charge_energy(-1), m_assignedbinid(-1), m_assigned_station_id(-1)
    {
    }
    CCharging::CCharging(int id, int time, int charging_energy, int assignedbinid, int assigned_station_id) : m_id(id), m_time(time), m_charge_energy(charging_energy), m_assignedbinid(assignedbinid), m_assigned_station_id(assigned_station_id)
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
    void CCharging::SetAssignedBinId(int binid, int charge_time, int charge_energy)
    {
        m_assignedbinid = binid;
        m_time = charge_time;
        m_charge_energy = charge_energy;
    }
    void CCharging::ClearAssignedBinId()
    {
        m_assignedbinid = -1;
        m_time = -1;
        m_charge_energy = -1;
    }
};
