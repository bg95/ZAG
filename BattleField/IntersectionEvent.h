#ifndef INTERSECTIONEVENT_H
#define INTERSECTIONEVENT_H

class BFObject;

//Information of an intersection
class IntersectionEvent
{
public:
    enum Boundary
    {
        LEFT, RIGHT, BOTTOM, TOP, NONE
    };
    const static double INVALID; //invalid time

    IntersectionEvent();
    IntersectionEvent(BFObject *_obj, Boundary _b, double _time);
    IntersectionEvent(BFObject *_obj1, BFObject *_obj2, double _time);
    //earlier than
    bool operator <(const IntersectionEvent &b) const;
    //identical to
    bool operator ==(const IntersectionEvent &b) const;
    union
    {
        struct
        {
            BFObject *obj;
            Boundary b;
        };
        struct
        {
            BFObject *obj1, *obj2;
        };
    };
    bool boundary; //true if it is a boundary intersection
    double time; //time when collision occurs (from now, so it's always negative). time == INVALID if this is a invalid collision
    bool ignored; //whether this intersections is ignored by BFRule
};

#include "BFObject/BFObject.h"

#endif // INTERSECTIONEVENT_H
