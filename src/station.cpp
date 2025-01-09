
#include "station.h"

namespace gap
{
    CStation::CStation() : m_id(-1), m_charge_efficiency(-1), m_occupation_rate(-1), m_overflow(-1)
    {
    }
    CStation::CStation(int id, int efficiency, int rate, int overflow) : m_id(id), m_charge_efficiency(efficiency), m_occupation_rate(rate), m_overflow(overflow)
    {
    }
    CStation::~CStation()
    {
    }
};
