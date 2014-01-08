#ifndef BFCONTROLLER_H
#define BFCONTROLLER_H

#include <Qt>
#include <set>
#include "../Vector2d.h"

class BFObject;
class BFManager;

enum BFControllerType
{
    BFC_NONE, BFC_HUMAN, BFC_AI, BFC_REMOTE
};

//A controller to control an object (abstract class)
//Only for local control. Remote control is done separately
class BFController
{
public:
    BFController(BFManager *_manager, BFObject *_obj);
    virtual ~BFController();
    virtual BFControllerType getType() const = 0;
    virtual void applyControl() = 0; //apply control to the object
    void setKeysAndMouse(std::set<Qt::Key> keyspressed, Vector2d mouseposition, Qt::MouseButtons mousebuttons);

protected:
    BFObject *obj;
    BFManager *manager;

    std::set<Qt::Key> keys;
    Vector2d mousepos;
    Qt::MouseButtons mousebut;
    bool keyPressed(Qt::Key key);

};

#include "../BFObject/BFObject.h"
#include "../BFManager.h"

#endif // BFCONTROLLER_H
