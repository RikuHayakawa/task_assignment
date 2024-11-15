/**
 * test.cpp
 * Author: Zhiyang Su
 * Created on 2014-08-10.
 */

#include "test.h"
#include "item.h"
#include "bin.h"
#include "knapsack.h"
#include "gap.h"
#include <iostream>
#include <fstream>

using namespace std;

namespace gap
{
    void TestKnapsack()
    {
        /* Get input data from testcaseforknapsack.txt
         * Format:
         * --------------------------------------
         * item_num bin_size
         * item_size1 item_size2 ...
         * item_profit1 item_profit2 ...
         * --------------------------------------
         */
        ifstream file("test/testcaseforknapsack.txt");
        int casenum = 1, itemnum, binsize;
        if (file.is_open())
        {
            while (file >> itemnum >> binsize)
            {
                cout << "Case " << casenum << ":" << endl;
                CKnapsack knapsack;
                CBin bin(1, binsize, 1); // todo: add max_size
                knapsack.SetBin(bin);
                for (int i = 0; i < itemnum; ++i)
                {
                    int weight;
                    file >> weight;
                    CItem item(i + 1, weight, -1, -1);
                    knapsack.AddItem(item);
                }
                for (int i = 0; i < itemnum; ++i)
                {
                    int profit;
                    file >> profit;
                    knapsack.m_items[i].m_profit = profit;
                }
                for (int i = 0; i < itemnum; ++i)
                {
                    int task_time;
                    file >> task_time;
                    knapsack.m_items[i].m_time = task_time;
                }
                knapsack.Print();
                knapsack.Dp();
                cout << "Max Profit:" << knapsack.m_maxprofit << endl;
                knapsack.PrintAssignment();
                cout << endl;
                ++casenum;
            }
            file.close();
        }
    }

    void TestGap()
    {
        /* Get input data from testcaseforgap.txt
         * Format:
         * --------------------------------------
         * item_num bin_num
         * bin_size1 bin_size2 ...
         *
         * sizematrix (item_num * bin_num)
         *
         * profitmatrix (item_num * bin_num)
         * --------------------------------------
         */
        ifstream file("test/testcaseforgap.txt");
        int casenum = 1, itemnum, binnum, stationnum;
        if (!file.is_open())
        {
            throw "File not found!";
        }

        while (file >> itemnum >> binnum >> stationnum)
        {
            cout << "Case " << casenum << ":" << endl;
            CGap gap;
            for (int i = 0; i < itemnum; ++i)
            {
                CItem item(i + 1, -1, -1, -1);
                item.m_cost = -1;
                gap.AddItem(item);
            }
            vector<int> sizes(binnum);
            for (int i = 0; i < binnum; ++i)
            {
                file >> sizes[i];
            }
            vector<int> max_sizes(binnum);
            for (int i = 0; i < binnum; ++i)
            {
                file >> max_sizes[i];
            }
            for (int i = 0; i < binnum; ++i)
            {
                CBin bin(i + 1, sizes[i], max_sizes[i]);
                gap.AddBin(bin);
            }
            vector<int> charge_efficiencies(stationnum);
            for (int i = 0; i < stationnum; ++i)
            {
                file >> charge_efficiencies[i];
                CStation station(i + 1, charge_efficiencies[i]);
                gap.AddStation(station);
            }
            for (int i = 0; i < itemnum; ++i)
            {
                vector<int> line(binnum);
                for (int j = 0; j < binnum; ++j)
                    file >> line[j];
                gap.m_sizematrix.push_back(line);
            }
            for (int i = 0; i < itemnum; ++i)
            {
                vector<int> line(binnum);
                for (int j = 0; j < binnum; ++j)
                    file >> line[j];
                gap.m_profitmatrix.push_back(line);
            }
            for (int i = 0; i < itemnum; ++i)
            {
                vector<int> line(binnum);
                for (int j = 0; j < binnum; ++j)
                    file >> line[j];
                gap.m_timematrix.push_back(line);
            }
            gap.Print();
            cout << endl;
            gap.Approximate();
            ++casenum;
        }
        file.close();
    }
};
