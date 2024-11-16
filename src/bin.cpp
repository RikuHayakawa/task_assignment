/**
 * bin.cpp
 * Author: Zhiyang Su
 * Created on 2014-08-10.
 */

#include "bin.h"

namespace gap
{
    CBin::CBin() : m_id(-1), m_size(-1), m_max_size(-1), m_energy_efficiency(-1)
    {
    }

    CBin::CBin(int id, int size, int max_size, int energy_efficiency) : m_id(id), m_size(size), m_max_size(max_size), m_energy_efficiency(energy_efficiency)
    {
    }

    CBin::~CBin()
    {
    }
};
