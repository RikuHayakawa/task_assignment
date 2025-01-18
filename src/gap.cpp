/**
 * cpp
 * Author: Zhiyang Su
 * Created on 2014-08-10.
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
            for (int i = 0; i < m_items.size(); ++i)
            {
                m_items[i].m_energy = m_sizematrix[i][j];
                m_items[i].m_workigtime = m_timematrix[i][j];
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
            knapsack.DpUnderConstraintSize(constaint_time);
            // Copy the knapsack results back to gap
            for (int i = 0; i < m_items.size(); ++i)
            {
                if (knapsack.m_items[i].m_assignedbinid != -1)
                    m_items[i].m_assignedbinid = knapsack.m_items[i].m_assignedbinid;
            }
            // PrintAssignment();
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
        m_necessary_charge_time_matrix = vector<vector<int>>(m_items.size(), vector<int>(m_bins.size(), 0));

        for (int j = 0; j < m_bins.size(); ++j)
        {
            CKnapsack knapsack;
            knapsack.SetBin(m_bins[j]);
            // rest items存在しない場合は終了
            if (m_rest_items.size() == 0)
            {
                cout << "No rest items" << endl;
                break;
            }

            vector<int> itemSizeIncludeCharging(m_rest_items.size(), 0);

            for (int i = 0; i < m_rest_items.size(); ++i)
            {
                m_rest_items[i].m_energy = m_sizematrix[m_rest_items[i].m_id - 1][j];
                m_rest_items[i].m_workigtime = m_timematrix[m_rest_items[i].m_id - 1][j];
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
                int charging_time = std::ceil(static_cast<double>(m_rest_items[i].m_energy) / GetMinChargeEfficiency());
                itemSizeIncludeCharging[i] = m_rest_items[i].m_workigtime + charging_time;
                m_necessary_charge_time_matrix[m_rest_items[i].m_id - 1][j] = charging_time;
            }
            knapsack.DpUnderConstraintTime(constaint_time, itemSizeIncludeCharging);
            // Copy the knapsack results back to gap
            for (int i = 0; i < m_rest_items.size(); ++i)
            {
                if (knapsack.m_items[i].m_assignedbinid != -1)
                {
                    m_rest_items[i].m_assignedbinid = knapsack.m_items[i].m_assignedbinid;
                }
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
        int charging_id = 1;
        for (int i = 0; i < m_bins.size(); ++i)
        {
            int task_total_time = 0;
            for (int j = 0; j < m_items.size(); ++j)
            {
                if (m_items[j].m_assignedbinid == m_bins[i].m_id)
                {
                    task_total_time += m_timematrix[m_items[j].m_id - 1][m_items[j].m_assignedbinid - 1];
                }
            }
            if (task_total_time < constaint_time)
            {

                CCharging charging(charging_id, constaint_time - task_total_time, (constaint_time - task_total_time) * GetMinChargeEfficiency(), m_bins[i].m_id, -1);
                AddCharging(charging);
                charging_id++;
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
                << m_items[i].m_id << "," << m_items[i].m_energy << "," << m_items[i].m_profit << "," << m_items[i].m_workigtime << " ";
        cout << endl;
        cout << "Bins (id, size, max_size):" << endl;
        for (int i = 0; i < m_bins.size(); ++i)
            cout << m_bins[i].m_id << "," << m_bins[i].m_size << "," << m_bins[i].m_max_size << " ";
        cout << endl;
        cout << "Stations (id, charge_efficiency):" << endl;
        for (int i = 0; i < m_stations.size(); ++i)
            cout << m_stations[i].m_id << "," << m_stations[i].m_charge_efficiency << " ";
        cout << endl;
        cout << "Charge time for guaranteed energy:" << endl;
        for (auto it = m_charge_time_for_guaranteed_energy.begin(); it != m_charge_time_for_guaranteed_energy.end(); ++it)
            cout << it->first << "," << it->second << endl;
        cout << endl;
        cout << "Time matrix:" << endl;
        for (int i = 0; i < m_timematrix.size(); ++i)
        {
            for (int j = 0; j < m_timematrix[0].size(); ++j)
                cout << m_timematrix[i][j] << " ";
            cout << endl;
        }
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
                m_items[items[i].m_id - 1].SetAssignedBinId(items[i].m_assignedbinid, m_timematrix[items[i].m_id - 1][items[i].m_assignedbinid - 1], m_sizematrix[items[i].m_id - 1][items[i].m_assignedbinid - 1]);
                m_bins[items[i].m_assignedbinid - 1].m_initial_assignment.addAssignment("task", items[i].m_id, m_timematrix[items[i].m_id - 1][items[i].m_assignedbinid - 1],
                                                                                        m_sizematrix[items[i].m_id - 1][items[i].m_assignedbinid - 1]);
            }
        }
    }

    void CGap::SetAssignmentForChargings(vector<CCharging> &chargings)
    {
        for (int i = 0; i < chargings.size(); ++i)
        {
            if (chargings[i].m_assignedbinid != -1)
            {
                chargings[i].SetAssignedBinId(chargings[i].m_assignedbinid, chargings[i].m_time, chargings[i].m_charge_energy);
                m_bins[chargings[i].m_assignedbinid - 1].m_initial_assignment.addAssignment("charging", chargings[i].m_id, chargings[i].m_time, chargings[i].m_charge_energy);
            }
        }
    }

    vector<int> CGap::updateInitialAllBinary()
    {
        // m_binsのbaiary optionを計算する
        vector<int> all_binary(constaint_time, 0);
        for (int i = 0; i < m_bins.size(); ++i)
        {
            m_bins[i].m_initial_assignment.m_binary_option = vector<int>(constaint_time, 0);
            int baianry_time_step = 0;

            for (int j = 0; j < m_bins[i].m_initial_assignment.m_assignment.size(); ++j)
            {
                // taskの場合0, chargingの場合1
                if (m_bins[i].m_initial_assignment.m_assignment[j].first == "task")
                {
                    for (int k = 0; k < m_timematrix[m_bins[i].m_initial_assignment.m_assignment[j].second - 1][i]; ++k)
                    {
                        m_bins[i].m_initial_assignment.m_binary_option[baianry_time_step] = 0;
                        baianry_time_step++;
                    }
                }
                else if (m_bins[i].m_initial_assignment.m_assignment[j].first == "charging")
                {
                    for (int k = 0; k < m_chargings[m_bins[i].m_initial_assignment.m_assignment[j].second - 1].m_time; ++k)
                    {
                        m_bins[i].m_initial_assignment.m_binary_option[baianry_time_step] = 1;
                        all_binary[baianry_time_step] += 1;
                        baianry_time_step++;
                    }
                }
            }
        }
        return all_binary;
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
