#include "station.h"
#include <iostream>
#include <vector>

namespace gap
{
    CStation::CStation() : m_id(-1), m_charge_efficiency(-1), m_occupation_rate(-1), m_overflow(-1)
    {
    }
    CStation::CStation(int id, int efficiency, int rate, int overflow, std::vector<int> occupation_per_time) : m_id(id), m_charge_efficiency(efficiency), m_occupation_rate(rate), m_overflow(overflow), m_occupation_per_time(occupation_per_time)
    {
    }
    CStation::~CStation()
    {
    }
    void CStation::Print()
    {
        std::cout << "Station id: " << m_id << " charge efficiency: " << m_charge_efficiency << " occupation rate: " << m_occupation_rate << " overflow: " << m_overflow << std::endl;
        std::cout << "Occupation per time: ";
        for (int i = 0; i < m_occupation_per_time.size(); i++)
        {
            std::cout << m_occupation_per_time[i] << " ";
        }
        std::cout << std::endl;
    }
};
