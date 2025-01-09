
#include "charging.h"

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
};
