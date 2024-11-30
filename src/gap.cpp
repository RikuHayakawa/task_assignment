/**
 * gap.cpp
 * Author: Zhiyang Su
 * Created on 2014-08-10.
 */

/**
 * todo
 * - アイテムに時間の概念を追加する（一度消したが、再度追加する）
 * - 充電制約に必要な情報
 * - ロボットと充電ステーションの関係、割り当てられているタスクの情報と実行時間、順序
 * 　  bin model を拡張して、アイテムの割り当てられた順番を記録する（実行順にアイテムのidを記録する、時間はアイテムをたどればわかる）
 * 　　例：　bin に　アイテム　itme_1, item_2, item_3 が割り当てられた場合  [item_1, item_2, item_3] 充電と区別するために、"item_"をつける.
 *          充電の場合は、"charging_"をつける
 * - Print
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

    void CGap::AddRestItem(CItem &item)
    {
        m_rest_items.push_back(item);
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

    void CGap::ApproximateForConstraintSize()
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
            // knapsack.Print();
            knapsack.DpUnderConstraintSize();
            // knapsack.PrintAssignment();
            // Copy the knapsack results back to gap
            for (int i = 0; i < m_items.size(); ++i)
            {
                if (knapsack.m_items[i].m_assignedbinid != -1)
                    m_items[i].m_assignedbinid = knapsack.m_items[i].m_assignedbinid;
            }
            PrintAssignment();
        }
        SetAssignmentForRobots();
    }

    void CGap::ApproximateForConstraintTime()
    {
        for (int j = 0; j < m_bins.size(); ++j)
        {
            CKnapsack knapsack;
            knapsack.SetBin(m_bins[j]);
            cout << endl
                 << "Iterative " << j + 1 << endl;

            // rest items存在しない場合は終了
            if (m_rest_items.size() == 0)
            {
                cout << "No rest items" << endl;
                break;
            }

            for (int i = 0; i < m_rest_items.size(); ++i)
            {
                m_rest_items[i].m_weight = m_sizematrix[i][j];
                if (m_rest_items[i].m_assignedbinid == -1)
                {
                    m_rest_items[i].m_profit = m_profitmatrix[i][j];
                }
                else
                {
                    m_rest_items[i].m_profit = m_profitmatrix[i][j] - m_profitmatrix[i][m_rest_items[i].m_assignedbinid - 1];
                }
                knapsack.AddItem(m_rest_items[i]);
                knapsack.m_items[i].m_assignedbinid = -1;
            }
            // knapsack.Print();
            knapsack.DpUnderConstraintTime(constaint_time);
            // knapsack.PrintAssignment();
            // Copy the knapsack results back to gap
            for (int i = 0; i < m_rest_items.size(); ++i)
            {
                if (knapsack.m_items[i].m_assignedbinid != -1)
                    m_rest_items[i].m_assignedbinid = knapsack.m_items[i].m_assignedbinid;
            }
            PrintAssignment();
        }
        SetAssignmentForRobots();
    }

    void CGap::Print()
    {
        cout << "Items(id, size, profit, time) : " << endl;
        for (int i = 0; i < m_items.size(); ++i)
            cout
                << m_items[i].m_id << "," << m_items[i].m_weight << "," << m_items[i].m_profit << "," << m_items[i].m_workigtime << " ";
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
        cout << "Constraint time:" << endl;
        cout << constaint_time << endl;
    }

    void CGap::PrintAssignment()
    {
        cout << "Gap assignment (itemid, binid):" << endl;
        for (int i = 0; i < m_items.size(); ++i)
            cout << m_items[i].m_id << "->" << m_items[i].m_assignedbinid << "  ";
        cout << endl;
    }

    void CGap::SetAssignmentForRobots()
    {
        for (int i = 0; i < m_items.size(); ++i)
        {
            if (m_items[i].m_assignedbinid != -1)
            {
                m_bins[m_items[i].m_assignedbinid - 1].addAssignment("task", m_items[i].m_id, m_items[i].m_workigtime);
            }
        }
        for (int i = 0; i < m_bins.size(); ++i)
        {
            cout << "Robot " << m_bins[i].m_id << " executes tasks:" << endl;
            m_bins[i].displayAssignments();
        }
    }
};
