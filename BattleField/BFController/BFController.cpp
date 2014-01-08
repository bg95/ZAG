#include "BFController.h"

BFController::BFController(BFManager *_manager, BFObject *_obj) :
    manager(_manager), obj(_obj)
{
    obj->controller = this;
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
