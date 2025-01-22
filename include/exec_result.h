

#include <iostream>
#include <fstream>
#include <vector>
#include <gap.h>

namespace exec_result
{

    class ExecResult
    {
    private:
        std::string m_test_id;                  // 識別子
        int m_total_tasks;                      // 総タスク
        int m_total_robots;                     // 総ロボット数
        int m_constraint_time;                  // 指定した実行時間
        int m_guaranteed_energy;                // 終了時の充電残量
        int m_station_capacity;                 // 充電ステーションの容量
        double m_gap_for_constraint_size_time;  // 段階1のアルゴリズムの実行時間
        double m_gap_for_constraint_time;       // 段階2のアルゴリズムの実行時間
        double m_schedule_planner_time;         // 段階3のアルゴリズム(充電計画)の実行時間
        double m_exec_time;                     // 全体の実行時間
        std::vector<int> m_initial_occupation;  // 初期の収容状況
        std::vector<int> m_schedule_occupation; // スケジュール後の収容状況

        // モデル内で計算
        std::tuple<int, double, double, double, int> m_initial_result;   // 初期結果(競合数, 標準偏差, 評価値, 平均収容率, 最大収容数)
        std::tuple<int, double, double, double, int> m_scheduled_result; // スケジュール結果(競合数, 標準偏差, 評価値, 平均収容率, 最大収容数)
        int m_assigned_tasks = 0;                                        // 割り当てられた総タスク数(完了できないタスクを含む)
        double m_initial_charging_average = 0;                           // 充電計画前の平均充電回数
        double m_scheduled_charging_average = 0;                         // 充電計画後の平均充電回数
        double m_initial_rest_energy_average = 0;                        // 充電計画前の平均充電残量
        double m_scheduled_rest_energy_average = 0;                      // 充電計画後の平均充電残量
        int m_initial_task_size = 0;                                     // 初期のタスク数
        int m_scheduled_task_size = 0;                                   // スケジュール後のタスク数
        int m_initial_task_value = 0;                                    // 初期のタスクの価値
        int m_scheduled_task_value = 0;                                  // スケジュール後のタスクの価値

        void calculateStationOccupation(std::vector<int> occupation_per_time, std::tuple<int, double, double, double, int> &result);
        bool isTestIdExists(const std::string &filename, const std::string &test_id);
        void outputResultsToFile(const std::string &filename, const std::string &test_id);
        void calculateTaskSizeAndValue(gap::CGap &gap_instance);

    public:
        ExecResult();
        ExecResult(std::string test_id, int total_tasks, int total_robots, int constraint_time, int guaranteed_energy, int station_capacity,
                   double gap_for_constraint_size_time, double gap_for_constraint_time, double schedule_planner_time, double exec_time, std::vector<int> initial_occupation,
                   std::vector<int> schedule_occupation, gap::CGap *gap_instance);
        virtual ~ExecResult();
        void Print();
    };
};