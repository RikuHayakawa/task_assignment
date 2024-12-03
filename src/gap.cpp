/**
 * cpp
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
#include <cmath>

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
        SetAssignmentForItems(m_items);
    }

    void CGap::ApproximateForConstraintTime()
    {
        for (int i = 0; i < m_items.size(); ++i)
        {
            if (m_items[i].m_assignedbinid == -1)
            {
                AddRestItem(m_items[i]);
            }
        }
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

            vector<int> itemSizeIncludeCharging(m_rest_items.size(), 0);

            for (int i = 0; i < m_rest_items.size(); ++i)
            {
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
                // 充電時間を追加
                int task_energy = m_rest_items[i].m_workigtime * m_bins[j].m_energy_efficiency;
                int charging_time = std::ceil(static_cast<double>(task_energy) / GetMinChargeEfficiency());
                itemSizeIncludeCharging[i] = m_rest_items[i].m_workigtime + charging_time;
            }
            knapsack.Print();
            knapsack.DpUnderConstraintTime(constaint_time, itemSizeIncludeCharging);
            // Copy the knapsack results back to gap
            for (int i = 0; i < m_rest_items.size(); ++i)
            {
                if (knapsack.m_items[i].m_assignedbinid != -1)
                    m_rest_items[i].m_assignedbinid = knapsack.m_items[i].m_assignedbinid;
            }
        }
        // m_rest_itemsの割り当てをm_itemsにコピーする
        for (int i = 0; i < m_rest_items.size(); ++i)
        {
            for (int j = 0; j < m_items.size(); ++j)
            {
                if (m_items[j].m_id == m_rest_items[i].m_id)
                {
                    m_items[j].m_assignedbinid = m_rest_items[i].m_assignedbinid;
                }
            }
        }
        // m_rest_itemsを割り当てる前に、chargingを割り当てる。ここでchargingはbinにすでに割り当てられているitemsとm_rest_itemsの間に割り当てられる。
        for (int i = 0; i < m_bins.size(); ++i)
        {
            int task_total_time = 0;
            for (int j = 0; j < m_items.size(); ++j)
            {
                if (m_items[j].m_assignedbinid == m_bins[i].m_id)
                {
                    task_total_time += m_items[j].m_workigtime;
                }
            }
            if (task_total_time < constaint_time)
            {
                CCharging charging(i + 1, constaint_time - task_total_time, m_bins[i].m_id, -1);
                AddCharging(charging);
            }
        }
        SetAssignmentForChargings(m_chargings);
        SetAssignmentForItems(m_rest_items);
    }

    void CGap::Print() // 入力データを表示する
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

    void CGap::PrintAssignment() // DPConstraintSizeの結果を表示する
    {
        cout << "Gap assignment (itemid, binid):" << endl;
        for (int i = 0; i < m_items.size(); ++i)
            cout << m_items[i].m_id << "->" << m_items[i].m_assignedbinid << "  ";
        cout << endl;
    }

    void CGap::SetAssignmentForItems(vector<CItem> &items)
    {
        for (int i = 0; i < items.size(); ++i)
        {
            if (items[i].m_assignedbinid != -1)
            {
                m_bins[items[i].m_assignedbinid - 1].addAssignment("task", items[i].m_id, items[i].m_workigtime);
            }
        }
        for (int i = 0; i < m_bins.size(); ++i)
        {
            cout << "Robot " << m_bins[i].m_id << " executes tasks:" << endl;
            m_bins[i].displayAssignments();
        }
    }

    void CGap::SetAssignmentForChargings(vector<CCharging> &chargings)
    {
        for (int i = 0; i < chargings.size(); ++i)
        {
            if (chargings[i].m_assignedbinid != -1)
            {
                m_bins[chargings[i].m_assignedbinid - 1].addAssignment("charging", chargings[i].m_id, chargings[i].m_time);
            }
        }
        for (int i = 0; i < m_bins.size(); ++i)
        {
            cout << "Robot " << m_bins[i].m_id << " executes chargings:" << endl;
            m_bins[i].displayAssignments();
        }
    }

    int CGap::GetMinChargeEfficiency()
    {
        int min_charge_efficiency = m_stations[0].m_charge_efficiency;
        for (int i = 1; i < m_stations.size(); ++i)
        {
            if (m_stations[i].m_charge_efficiency < min_charge_efficiency)
            {
                min_charge_efficiency = m_stations[i].m_charge_efficiency;
            }
        }
        return min_charge_efficiency;
    }

};
