#include "BFController.h"

BFController::BFController(BFObject *_obj) :
    obj(_obj)
{
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
