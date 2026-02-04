/*****************************************************************************
* @brief : guid
* 全局唯一标识符
* @author : acedtang
* @date : 2021/4/6
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __GUID_H
#define __GUID_H

#include <string>
#include "Define.h"

namespace Sun
{
    struct Guid
    {
    public:
        /** Holds the first component. */
        uint32 A = 0;
        /** Holds the second component. */
        uint32 B = 0;
        /** Holds the third component. */
        uint32 C = 0;
        /** Holds the fourth component. */
        uint32 D = 0;

        friend bool operator < (const Guid& g1, const Guid& g2);
        friend bool operator == (const Guid& g1, const Guid& g2);
        friend bool operator != (const Guid& g1, const Guid& g2);
        inline void clear() {
            A = B = C = D = 0;
        }
        inline bool empty() const {
            return (A == 0) && (B == 0) && (C == 0) && (D == 0);
        }
    };


    struct GuidHashFunc
    {
        std::size_t operator()(const Guid& key) const
        {
            using std::size_t;
            using std::hash;
            //this hash is easy but may not be well ,change to a better one ,FIXME!
            return ((hash<int>()(key.A)
                ^ (hash<int>()(key.B) << 1)) >> 1)
                ^ (hash<int>()(key.C) << 1)
                ^ ((hash<int>()(key.D) <<1)>>1);
        }
    };

    struct GuidEqual
    {
        bool operator () (const Guid& lhs, const Guid& rhs) const
        {
            return lhs.A == rhs.A && lhs.B == rhs.B && lhs.C == rhs.C && lhs.D == rhs.D;
        }
    };

    inline bool operator< (const Guid & g1, const Guid & g2) {
        if (g1.A < g2.A) {
            return true;
        }
        else if (g1.A > g2.A) {
            return false;
        }
        if (g1.B < g2.B)
            return true;
        else if (g1.B > g2.B)
            return false;
        if (g1.C < g2.C)
            return true;
        else if (g1.C > g2.C)
            return false;
        if (g1.D < g2.D)
            return true;
        else
            return false;
    }

    bool operator == (const Guid& g1, const Guid& g2);

}


#endif 

