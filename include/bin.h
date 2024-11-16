/**
 * bin.h
 * Author: Zhiyang Su
 * Created on 2014-08-10.
 */

#ifndef BIN_H
#define BIN_H

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
        CBin();
        CBin(int id, int size, int max_size, int energy_efficiency);
        virtual ~CBin();
    };
};

#endif
