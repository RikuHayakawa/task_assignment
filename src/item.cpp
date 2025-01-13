/**
 * item.cpp
 * Author: Zhiyang Su
 * Created on 2014-08-10.
 */

#include "item.h"

namespace gap
{
    CItem::CItem() : m_id(-1), m_energy(-1), m_profit(-1), m_cost(-1), m_workigtime(-1),
                     m_assignedbinid(-1)
    {
    }

    CItem::CItem(int id, int weight, int profit, int workigtime) : m_id(id), m_energy(weight), m_profit(profit), m_workigtime(workigtime), m_assignedbinid(-1)
    {
        UpdateCost();
    }

    CItem::~CItem()
    {
    }

    void CItem::UpdateCost()
    {
        m_cost = m_profit / m_energy;
    }

    void CItem::SetAssignedBinId(int binid, int workigtime, int energy)
    {
        m_assignedbinid = binid;
        m_workigtime = workigtime;
        m_energy = energy;
    }

    void CItem::ClearAssignedBinId()
    {
        m_assignedbinid = -1;
        m_workigtime = -1;
        m_energy = -1;
    }

};
