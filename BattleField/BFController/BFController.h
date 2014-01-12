#ifndef BFCONTROLLER_H
#define BFCONTROLLER_H

#include <Qt>
#include <set>
#include "../Vector2d.h"

class BFObject;
class BFManager;

#include "../BFObject/BFObject.h"

enum BFControllerType
{
    BFC_NONE, BFC_HUMAN, BFC_AI, BFC_REMOTE
};

//A controller to control an object (abstract class)
//Only for local control. Remote control is done separately
class BFController
{
public:
    BFController(BFManager *_manager, BFObjectID _obj);
    BFController(BFManager *_manager, std::vector<BFObjectID> _obj);
    virtual ~BFController();
    //virtual BFControllerType getType() const = 0;
    //virtual void applyControl() = 0; //apply control to the object

    //apply control to the object and return a vector of ControlEvent's
    virtual std::vector<ControlEvent> &getControl();
    //used to set the value of keys, mousepos, mousebut. Should not be overwritten
    void setKeysAndMouse(std::set<Qt::Key> keyspressed, Vector2d mouseposition, Qt::MouseButtons mousebuttons);

    //returns true if no objects is on the battlefield
    bool lostAllObjects();
    //return the object controlled by human, if any.
    BFObject *getObjectControlledByHuman();
protected:
    BFManager *manager;
    std::vector<BFObjectID> objid;
    std::vector<ControlEvent> controlevents;

    std::set<Qt::Key> keys;
    Vector2d mousepos;
    Qt::MouseButtons mousebut;
    bool keyPressed(Qt::Key key);
    BFObject *getObjectPointer(BFObjectID id);
    BFObject *getPrincipalObjectPointer(); //for when objid.size() == 1

};

#include "../BFManager.h"

#endif // BFCONTROLLER_H
