
#include "charging.h"

namespace gap
{
    CCharging::CCharging() : m_id(-1), m_time(-1)
    {
    }
    CCharging::CCharging(int id, int time, int assigned_robot_id, int assigned_station_id) : m_id(id), m_time(time), assigned_robot_id(assigned_robot_id), assigned_station_id(assigned_station_id)
    {
    }
    CCharging::~CCharging()
    {
    }
};
