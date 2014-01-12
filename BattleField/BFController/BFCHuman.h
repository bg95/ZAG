#ifndef BFCHUMAN_H
#define BFCHUMAN_H

#include <Qt>
#include "BFController.h"
#include "BFCRandomShootDodge.h"
#include "../Vector2d.h"

//A controller controled directly by human (mouse and keyboard)
//Seen as an AI that strictly follows human instructions
class BFCHuman : public BFCRandomShootDodge
{
public:
    static Qt::Key XNegKey, XPosKey, YNegKey, YPosKey;

    BFCHuman(BFManager *_manager, BFObjectID _obj);
    BFCHuman(BFManager *_manager, std::vector<BFObjectID> _obj);
    virtual ~BFCHuman();
    //virtual BFControllerType getType() const;
    virtual std::vector<ControlEvent> &getControl();
    virtual void applyControl();

};

#endif // BFCHUMAN_H
