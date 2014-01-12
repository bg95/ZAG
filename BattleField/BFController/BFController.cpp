#include "BFController.h"

BFController::BFController(BFManager *_manager, BFObjectID _obj) :
    manager(_manager), objid()
{
    objid.push_back(_obj);
}

BFController::BFController(BFManager *_manager, std::vector<BFObjectID> _obj) :
    manager(_manager), objid(_obj)
{
}

BFController::~BFController()
{
}

std::vector<ControlEvent> &BFController::getControl()
{
    return controlevents;
}

void BFController::setKeysAndMouse(std::set<Qt::Key> keyspressed, Vector2d mouseposition, Qt::MouseButtons mousebuttons)
{
    keys = keyspressed;
    mousepos = mouseposition;
    mousebut = mousebuttons;
}

bool BFController::lostAllObjects()
{
    if (objid.size() == 0)
        return true;
    for (auto iter = objid.begin(); iter != objid.end(); iter++)
        if (getObjectPointer(*iter))
            return false;
    return true;
}

bool BFController::keyPressed(Qt::Key key)
{
    keys.find(key); //not completed
    std::set<Qt::Key>::iterator iter;
    for (iter = keys.begin(); iter != keys.end(); iter++)
        if ((*iter) == key)
            return true;
    return false;
}

BFObject *BFController::getObjectPointer(BFObjectID id)
{
    return manager->getFactory()->objectByID(id);
}

BFObject *BFController::getPrincipalObjectPointer()
{
    if (objid.size() == 0)
        return 0;
    return getObjectPointer(objid[0]);
}

BFObject *BFController::getObjectControlledByHuman()
{
    for(auto iter = objid.begin(); iter != objid.end(); iter ++)
    {
        if( (* (getObjectPointer(*iter)) )["isHumanControlled"].toBool() )
        {
            qDebug("Find the object controlled!");
            return getObjectPointer(*iter);
        }
    }
    return NULL;
}
