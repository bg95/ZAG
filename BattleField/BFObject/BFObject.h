#ifndef BFOBJECT_H
#define BFOBJECT_H

#include <QGLWidget>
#include <QIODevice>
#include <QVariant>
#include <string>
#include <map>
#include "../Vector2d.h"
//#include "BFManager.h"

const double PI = 3.14159265358979323846264338327950288419716939937510;

class BFManager;
class BFController;

enum BFObjectType
{
    BFO_CIRCLE, BFO_COLORED_CIRCLE
};

class BFObject
{
public:

    static const std::string empty_string;
    static long count;
    static void readStdString(QIODevice *device, std::string &str); //read a string from device
    static void writeStdString(QIODevice *device, const std::string &str); //write a string to device

    BFObject();
    //BFObject(BFManager *_manager = 0);
    virtual ~BFObject();
    virtual BFObject *newObject() = 0;
    virtual BFObject *duplicate();

    virtual void draw(QGLWidget *) = 0;
    virtual BFObjectType getType() const = 0;
    virtual double getRoughRadius() const = 0; //The radius of a circle centered at the object's center, large enough to cover the whole object, used for Quadtree
    virtual Vector2d getPosition() const = 0;
    //virtual Vector2d getVelocity() const = 0;

    virtual void move(double time) = 0;

    //encode, decode not tested
    virtual void encode(QIODevice *device); //must be called if overwritten by subclasses
    virtual void decode(QIODevice *device); //must be called if overwritten by subclasses

    friend class BFController;
    BFController *getController();

    long getID();
    bool operator <(BFObject &b)
    {
        return id < b.id;
    }

    //set/getProperty not tested
    void setProperty(const std::string &prop, const QVariant &val);
    const QVariant &getProperty(const std::string &prop);
    //void setProperty(const std::string &prop, const std::string &val);
    const QVariant &operator[](const std::string &prop);

private:
    long id;
    //BFManager *manager;
    std::string info;  //unused
    std::map<std::string, QVariant> properties;
    BFController *controller;

};

#include "../BFManager.h"
#include "../BFController/BFController.h"

#endif // BFOBJECT_H
