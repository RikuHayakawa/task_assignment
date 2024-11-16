/**
 * gap.cpp
 * Author: Zhiyang Su
 * Created on 2014-08-10.
 */

#include "gap.h"
#include "item.h"
#include "bin.h"
#include "knapsack.h"
#include <iostream>

using namespace std;

namespace gap
{
    CGap::CGap()
    {
    }

    CGap::~CGap()
    {
    }

    void CGap::AddItem(CItem &item)
    {
        m_items.push_back(item);
    }

    void CGap::AddBin(CBin &bin)
    {
        m_bins.push_back(bin);
    }

    void CGap::AddStation(CStation &station)
    {
        m_stations.push_back(station);
    }

    void CGap::AddCharging(CCharging &charging)
    {
        m_chargings.push_back(charging);
    }

    void CGap::Approximate()
    {
        for (int j = 0; j < m_bins.size(); ++j)
        {
            CKnapsack knapsack;
            knapsack.SetBin(m_bins[j]);
            cout << endl
                 << "Iterative " << j + 1 << endl;
            for (int i = 0; i < m_items.size(); ++i)
            {
                m_items[i].m_weight = m_sizematrix[i][j];
                if (m_items[i].m_assignedbinid == -1)
                {
                    m_items[i].m_profit = m_profitmatrix[i][j];
                }
                else
                {
                    m_items[i].m_profit = m_profitmatrix[i][j] - m_profitmatrix[i][m_items[i].m_assignedbinid - 1];
                }
                knapsack.AddItem(m_items[i]);
                knapsack.m_items[i].m_assignedbinid = -1;
            }
            knapsack.Print();
            knapsack.Dp();
            knapsack.PrintAssignment();
            // Copy the knapsack results back to gap
            for (int i = 0; i < m_items.size(); ++i)
            {
                if (knapsack.m_items[i].m_assignedbinid != -1)
                    m_items[i].m_assignedbinid = knapsack.m_items[i].m_assignedbinid;
            }
            PrintAssignment();
        }
    }

    void CGap::Print()
    {
        cout << "Items:" << endl;
        for (int i = 0; i < m_items.size(); ++i)
            cout << m_items[i].m_id << " ";
        cout << endl;
        cout << "Bins (id, size, max_size, energy_efficiency):" << endl;
        for (int i = 0; i < m_bins.size(); ++i)
            cout << m_bins[i].m_id << "," << m_bins[i].m_size << "," << m_bins[i].m_max_size << "," << m_bins[i].m_energy_efficiency << " ";
        cout << endl;
        cout << "Stations (id, charge_efficiency):" << endl;
        for (int i = 0; i < m_stations.size(); ++i)
            cout << m_stations[i].m_id << "," << m_stations[i].m_charge_efficiency << " ";
        cout << endl;
        cout << "Chargings (id, time) :" << endl;
        for (int i = 0; i < m_chargings.size(); ++i)
        {
            cout << m_chargings[i].m_id << "," << m_chargings[i].m_time << " ";
        }
        cout << endl;
        cout << "Size matrix:" << endl;
        for (int i = 0; i < m_sizematrix.size(); ++i)
        {
            for (int j = 0; j < m_sizematrix[0].size(); ++j)
                cout << m_sizematrix[i][j] << " ";
            cout << endl;
        }
        cout << "Profit matrix:" << endl;
        for (int i = 0; i < m_profitmatrix.size(); ++i)
        {
            for (int j = 0; j < m_profitmatrix[0].size(); ++j)
                cout << m_profitmatrix[i][j] << " ";
            cout << endl;
        }
        cout << "Time matrix:" << endl;
        for (int i = 0; i < m_timematrix.size(); ++i)
        {
            for (int j = 0; j < m_timematrix[0].size(); ++j)
                cout << m_timematrix[i][j] << " ";
            cout << endl;
        }
    }

    void CGap::PrintAssignment()
    {
        cout << "Gap assignment (itemid, binid):" << endl;
        for (int i = 0; i < m_items.size(); ++i)
            cout << m_items[i].m_id << "->" << m_items[i].m_assignedbinid << "  ";
        cout << endl;
    }
};
