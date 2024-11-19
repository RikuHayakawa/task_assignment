#ifndef BIN_H
#define BIN_H

#include <vector>
#include <string>
#include <utility> // std::pair

namespace gap
{
    class CBin
    {
    public:
        int m_id;
        int m_size;
        int m_max_size;
        int m_energy_efficiency;

    public:
        // Change the type of m_assignments to store pairs instead of Assignment objects
        std::vector<std::pair<std::string, int>> m_assignments; // To hold pairs of (name, id)

        CBin();
        CBin(int id, int size, int max_size, int energy_efficiency);
        virtual ~CBin();

        void removeAssignment(int pos);                             // Remove an assignment at specified position
        void addAssignment(const std::string& name, const int id );  // Add a new assignment (pair)
        void swapAssignments(int startIndex, int endIndex);                   // Swap two assignments
        void displayAssignments() const;                            // Display all assignments
    };
};

#endif
