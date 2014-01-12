#ifndef BFFACTORY_H
#define BFFACTORY_H

#include <map>
#include <string>
#include <typeindex>

#include "BFObject/BFObject.h"
#include "BFController/BFController.h"

//A factory to create/destruct/find objects
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
    BFObject *replaceObject(BFObjectID id, BFObjectType type); //not tested

    void encodeObject(BFObject *o, QIODevice *device);
    BFObject *decodeNewObject(QIODevice *device);
    BFObject *decodeNewObject(QByteArray ba);
    BFObject *decodeReplaceObject(QIODevice *device); //not tested

    void clear();

    BFObject *objectByID(BFObjectID id);

private:
    std::map<BFObjectType, BFObject *> prototypes;
    std::map<BFObjectID, BFObject *> objects;

};

#endif // BFFACTORY_H
