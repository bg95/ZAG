#ifndef BFFACTORY_H
#define BFFACTORY_H

#include <map>
#include <string>
#include <typeindex>

#include "BFObject/BFObject.h"
#include "BFController/BFController.h"

//Should be able to find an object by id (for constructing bullets e.g.)
//Avoid trasmitting addresses while networking
//Should store all object prototypes (find classes by std::string or BFObjectType) and manage all objects (by id)
//All construction / destruction must be through this factory
class BFFactory
{
public:
    BFFactory();
    ~BFFactory();

    BFObject *newObject(BFObjectType type);
    void deleteObject(BFObject *o);
    BFObject *replaceObject(long id, BFObjectType type); //not tested

    void encodeObject(BFObject *o, QIODevice *device);
    BFObject *decodeNewObject(QIODevice *device);
    BFObject *decodeReplaceObject(long id, QIODevice *device); //not tested

    void clear();

private:
    std::map<BFObjectType, BFObject *> prototypes;
    std::map<long, BFObject *> objects;

};

#endif // BFFACTORY_H
