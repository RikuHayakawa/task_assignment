
#include "station.h"

namespace gap
{
    CStation::CStation() : m_id(-1), m_charge_efficiency(-1)
    {
    }
    CStation::CStation(int id, int efficiency) : m_id(id), m_charge_efficiency(efficiency)
    {
    }
    CStation::~CStation()
    {
    }
};
