/**
 * knapsack.cpp
 * Author: Zhiyang Su
 * Created on 2014-08-10.
 */

#include "knapsack.h"
#include "item.h"
#include "bin.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

namespace gap
{
    CKnapsack::CKnapsack() : m_maxprofit(-1)
    {
    }

    CKnapsack::~CKnapsack()
    {
    }

    void CKnapsack::AddItem(CItem &item)
    {
        m_items.push_back(item);
    }

    void CKnapsack::SetBin(CBin &bin)
    {
        m_bin = bin;
    }

    void CKnapsack::Print()
    {
        cout << "Bin:" << m_bin.m_size << endl;
        cout << "Items(id, weight, profit, time):" << endl;
        for (int i = 0; i < m_items.size(); ++i)
        {
            cout << m_items[i].m_id << " " << m_items[i].m_weight << " " << m_items[i].m_profit << " " << m_items[i].m_workigtime << endl;
        }
    }

    void CKnapsack::PrintAssignment()
    {
        cout << "Knapsack assignment (itemid, binid):" << endl;
        for (int i = 0; i < m_items.size(); ++i)
        {
            if (m_items[i].m_assignedbinid != -1)
                cout << m_items[i].m_id << "->" << m_items[i].m_assignedbinid << "  ";
        }
        cout << endl;
    }

    // binの容量制約のみを考慮してDPを行う
    void CKnapsack::DpUnderConstraintSize()
    {
        /* Here we do not optimize the space usage from O(nW) to O(W),
         * since we need to trace the chose items later.
         */
        vector<vector<int>> d(m_items.size() + 1, vector<int>(m_bin.m_size + 1, 0));
        for (int i = 1; i <= m_items.size(); ++i)
        {
            for (int j = 1; j <= m_bin.m_size; ++j)
            {
                if (m_items[i - 1].m_weight <= j)
                {
                    int t = d[i - 1][j - m_items[i - 1].m_weight] + m_items[i - 1].m_profit;
                    if (t > d[i - 1][j])
                        d[i][j] = t;
                    else
                        d[i][j] = d[i - 1][j];
                }
                else
                    d[i][j] = d[i - 1][j];
            }
        }
        m_maxprofit = d[m_items.size()][m_bin.m_size];
        // Trace the chose items
        int w = m_bin.m_size;
        for (int i = m_items.size(); i > 0; --i)
        {
            if (d[i][w] != d[i - 1][w])
            {
                // Item i is chose, continue tracing d[i - 1][w - w(i)]
                m_items[i - 1].m_assignedbinid = m_bin.m_id;
                w -= m_items[i - 1].m_weight;
            }
        }

        // Print the dp matrix
        //     for(int i = 1; i <= m_items.size(); ++i)
        //     {
        //         for(int j = 0; j <= m_bin.m_size; ++j)
        //             cout<<d[i][j]<<" ";
        //         cout<<endl;
        //     }
    }

    // todo: ロボットの作業時間の制約を満たすように、DPを行う。ただし、bin.sizeの制約はない。m_sizematrixで消費した分だけchargingを追加する。
    void CKnapsack::DpUnderConstraintTime(const int constraint_time)
    {
        // 制約時間が既存の作業時間を下回る場合、エラーを出力して処理を終了
        if (constraint_time <= m_bin.m_total_time)
        {
            std::cerr << "Error: Constraint time (" << constraint_time
                      << ") is less than or equal to the total assigned time ("
                      << m_bin.m_total_time << "). DP cannot proceed." << std::endl;
            m_maxprofit = 0;
            return;
        }

        // todo: アルゴリズム内でm_total_timeを更新する必要性がある
        int max_time = constraint_time - m_bin.m_total_time;
        vector<vector<int>> d(m_items.size() + 1, vector<int>(m_bin.m_size + 1, 0));
        for (int i = 1; i <= m_items.size(); ++i)
        {
            for (int j = 1; j <= max_time; ++j)
            {
                // m_timeがjを超える場合は、アイテムiを選択できない。
                if (m_items[i - 1].m_workigtime <= j)
                {
                    int t = d[i - 1][j - m_items[i - 1].m_workigtime] + m_items[i - 1].m_profit;
                    if (t > d[i - 1][j])
                        d[i][j] = t;
                    else
                        d[i][j] = d[i - 1][j];
                }
                else
                    d[i][j] = d[i - 1][j];
            }
        }
        m_maxprofit = d[m_items.size()][constraint_time - m_bin.m_total_time];
        // Trace the chose items
        int w = constraint_time - m_bin.m_total_time;
        for (int i = m_items.size(); i > 0; --i)
        {
            if (d[i][w] != d[i - 1][w])
            {
                // Item i is chose, continue tracing d[i - 1][w - w(i)]
                m_items[i - 1].m_assignedbinid = m_bin.m_id;
                w -= m_items[i - 1].m_workigtime;
            }
        }
    }

    void CKnapsack::Greedy()
    {
    }
};
