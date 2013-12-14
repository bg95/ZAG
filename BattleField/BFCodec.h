#ifndef BFCODEC_H
#define BFCODEC_H

#include "BFObject/BFObject.h"
#include "BFController/BFController.h"

class BFCodec
{
public:
    BFCodec();
    void encodeObject(BFObject *o, QIODevice *device);
    BFObject *decodeNewObject(QIODevice *device);
};

#endif // BFCODEC_H
