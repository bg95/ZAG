#include "BFObject.h"

long BFObject::count = 0;

BFObject::BFObject(BFManager *_manager)
    :manager(_manager), info()
{
    id = count;
    count++;
}

BFObject::~BFObject()
{
}

bool BFObject::intersectWith(BFObject *b)
{
    return manager->intersecting(this, b);
}

void BFObject::setInfo(std::string str)
{
    info = str;
}

const std::string &BFObject::getInfo() const
{
    return info;
}

void BFObject::setProperty(const std::string &prop, const std::string &val)
{
    properties[prop] = val;
}

const std::string &BFObject::getProperty(const std::string &prop)
{
    return properties[prop];
}
