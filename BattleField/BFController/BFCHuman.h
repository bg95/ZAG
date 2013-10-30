#ifndef BFCHUMAN_H
#define BFCHUMAN_H

#include <Qt>
#include "BFController.h"
#include "../Vector2d.h"

class BFCHuman : public BFController
{
public:
    static Qt::Key XNegKey, XPosKey, YNegKey, YPosKey;

    BFCHuman(BFObject *_obj);
    virtual ~BFCHuman();
    virtual BFControllerType getType() const;
    virtual void applyControl();

    void setKeysAndMouse(std::set<Qt::Key> keyspressed, Vector2d mouseposition, Qt::MouseButtons mousebuttons);

private:
    std::set<Qt::Key> keys;
    Vector2d mousepos;
    Qt::MouseButtons mousebut;
    bool keyPressed(Qt::Key key);

};

#endif // BFCHUMAN_H
