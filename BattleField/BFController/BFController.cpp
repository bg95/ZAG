#include "BFController.h"

BFController::BFController(BFManager *_manager, BFObjectID _obj) :
    manager(_manager), objid(_obj)
{
    BFObject *obj = getObjectPointer();
    if (!obj)
        return;
    //obj->controller = this;
}

BFController::~BFController()
{
}

void BFController::setKeysAndMouse(std::set<Qt::Key> keyspressed, Vector2d mouseposition, Qt::MouseButtons mousebuttons)
{
    keys = keyspressed;
    mousepos = mouseposition;
    mousebut = mousebuttons;
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

BFObject *BFController::getObjectPointer()
{
    return manager->getFactory()->objectByID(objid);
}
