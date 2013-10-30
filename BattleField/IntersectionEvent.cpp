#include "IntersectionEvent.h"

IntersectionEvent::IntersectionEvent()
    :obj1(0), obj2(0), boundary(false), time(INVALID)
{
}

IntersectionEvent::IntersectionEvent(BFObject *_obj, Boundary _b, double _time)
    :obj(_obj), b(_b), boundary(true), time(_time)
{
}

IntersectionEvent::IntersectionEvent(BFObject *_obj1, BFObject *_obj2, double _time)
    :obj1(_obj1), obj2(_obj2), boundary(false), time(_time)
{
}
