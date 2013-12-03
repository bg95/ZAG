#ifndef BFOBJECT_H
#define BFOBJECT_H

#include <QGLWidget>
#include <QIODevice>
#include <string>
#include <map>
#include "../Vector2d.h"
//#include "BFManager.h"

class BFManager;

enum BFObjectType
{
    BFO_CIRCLE
};

class BFObject
{
public:
    static const std::string empty_string;
    static long count;

    BFObject(BFManager *_manager);
    virtual ~BFObject();
    virtual void draw(QGLWidget *) = 0;
    virtual BFObjectType getType() const = 0;
    virtual double getRoughRadius() const = 0; //The radius of a circle centered at the object's center, large enough to cover the whole object, used for Quadtree
    virtual Vector2d getPosition() const = 0;

    virtual void move(double time) = 0;

    //encode, decode not tested
    virtual void encode(QIODevice *device); //must be called if overwritten by subclasses
    virtual void decode(QIODevice *device); //must be called if overwritten by subclasses

    bool intersectWith(BFObject *);
    bool operator <(BFObject &b)
    {
        return id < b.id;
    }

    //unused
    void setInfo(std::string str);
    const std::string &getInfo() const;

    //set/getProperty not tested
    void setProperty(const std::string &prop, const std::string &val);
    const std::string &getProperty(const std::string &prop);

private:
    long id;
    BFManager *manager;
    std::string info;  //unused
    std::map<std::string, std::string> properties;

    void readStdString(QIODevice *device, std::string &str); //read a string from device
    void writeStdString(QIODevice *device, const std::string &str); //write a string to device

};

#include "../BFManager.h"

#endif // BFOBJECT_H
