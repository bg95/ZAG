#ifndef BFFACTORY_H
#define BFFACTORY_H

#include "BFObject/BFObject.h"
#include "BFController/BFController.h"

class BFFactory
{
public:
    BFFactory();
    void encodeObject(BFObject *o, QIODevice *device);
    BFObject *decodeNewObject(QIODevice *device);
};

#endif // BFFACTORY_H
