#ifndef BFCONTROLLER_H
#define BFCONTROLLER_H

#include <Qt>
#include <set>
#include "../Vector2d.h"

class BFObject;

enum BFControllerType
{
    BFC_NONE, BFC_HUMAN, BFC_AI, BFC_REMOTE
};

//A controller to control an object (abstract class)
//Only for local control. Remote control is done separately
class BFController
{
public:
    BFController(BFObject *_obj);
    virtual ~BFController();
    virtual BFControllerType getType() const = 0;
    virtual void applyControl() = 0; //apply control to the object
    void setKeysAndMouse(std::set<Qt::Key> keyspressed, Vector2d mouseposition, Qt::MouseButtons mousebuttons);

protected:
    BFObject *obj;

    std::set<Qt::Key> keys;
    Vector2d mousepos;
    Qt::MouseButtons mousebut;
    bool keyPressed(Qt::Key key);

};

#include "../BFObject/BFObject.h"

#endif // BFCONTROLLER_H
