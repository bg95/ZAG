#ifndef BFCHUMANANDRSD_H
#define BFCHUMANANDRSD_H

#include "BFController.h"

class BFCHumanAndRSD : public BFController
{
public:
    BFCHumanAndRSD(BFManager *_manager, BFObjectID _obj);
    BFCHumanAndRSD(BFManager *_manager, std::vector<BFObjectID> _obj);
    virtual ~BFCHumanAndRSD();
    virtual std::vector<ControlEvent> &getControl();

private:
    void init();
    void clearControllers();
    void appendControlEvents(std::vector<ControlEvent> &events);
    std::vector<BFController *> controllers;

};

#endif // BFCHUMANANDRSD_H
