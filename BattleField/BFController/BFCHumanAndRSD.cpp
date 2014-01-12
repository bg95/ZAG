#include "BFCHuman.h"
#include "BFCRandomShootDodge.h"

#include "BFCHumanAndRSD.h"

BFCHumanAndRSD::BFCHumanAndRSD(BFManager *_manager, BFObjectID _obj) :
    BFController(_manager, _obj)
{
    init();
}

BFCHumanAndRSD::BFCHumanAndRSD(BFManager *_manager, std::vector<BFObjectID> _obj) :
    BFController(_manager, _obj)
{
    init();
}

BFCHumanAndRSD::~BFCHumanAndRSD()
{
    clearControllers();
}

std::vector<ControlEvent> &BFCHumanAndRSD::getControl()
{
    controlevents.clear();

    for (auto iter = controllers.begin(); iter != controllers.end(); iter++)
    {
        (*iter)->setKeysAndMouse(keys, mousepos, mousebut);
        appendControlEvents((*iter)->getControl());
    }

    //qDebug("Control %d", (int)controlevents.size());
    for (auto iter = controlevents.begin(); iter != controlevents.end(); iter++)
    {
        //qDebug("  %lf,%lf", (*iter).acc.x, (*iter).acc.y);
    }
    return controlevents;
}

void BFCHumanAndRSD::init()
{
    if (objid.size() == 0)
        return;
    auto iter = objid.begin();
    for (iter = objid.begin(); iter != objid.end(); iter++)
    {
        BFObject *pobj = getObjectPointer(*iter);
        if ((*pobj)["isHumanControl"].toBool())
        {
            break;
        }
    }
    if (iter != objid.end())
    {
        BFObjectID tid;
        tid = *iter;
        *iter = objid[0];
        objid[0] = tid;
    }
    controllers.clear();

    iter = objid.begin();
    controllers.push_back(new BFCHuman(manager, *iter));
    iter++;
    while (iter != objid.end())
    {
        controllers.push_back(new BFCRandomShootDodge(manager, *iter));
        iter++;
    }
}

void BFCHumanAndRSD::clearControllers()
{
    for (auto iter = controllers.begin(); iter != controllers.end(); iter++)
        delete *iter;
    controllers.clear();
}

void BFCHumanAndRSD::appendControlEvents(std::vector<ControlEvent> &events)
{
    for (auto iter = events.begin(); iter != events.end(); iter++)
        controlevents.push_back(*iter);
}
