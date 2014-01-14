#ifndef BFOBJECT_H
#define BFOBJECT_H

#include <QGLWidget>
#include <QIODevice>
#include <QVariant>
#include <string>
#include <typeinfo>
#include <map>
#include "../Vector2d.h"
//#include "BFManager.h"

const double PI = 3.14159265358979323846264338327950288419716939937510;

class BFManager;
class BFController;
class ControlEvent;

typedef size_t BFObjectType;
#define typehash(TYPE) (typeid(TYPE).hash_code())
typedef long BFObjectID;
/*
enum BFObjectType
{
    BFO_CIRCLE = 2, BFO_COLORED_CIRCLE = 3
};*/
enum BFObjectShape
{
    BFO_CIRCULAR = 0
};

//A class representing the objects on the battlefield
class BFObject
{
public:
    //some constants for convenience
    static const std::string empty_string;
    static const QVariant QVariant_Invalid;
    //the number of objects in history
    static BFObjectID count;
    //encoding/decoding some commonly used classes
    static void readStdString(QIODevice *device, std::string &str); //read a string from device
    static void writeStdString(QIODevice *device, const std::string &str); //write a string to device
    static void readQVariant(QIODevice *device, QVariant &var); //read a QVariant
    static void writeQVariant(QIODevice *device, const QVariant &var); //write a QVariant
    static void readVector2d(QIODevice *device, Vector2d &vec); //read a Vector2d
    static void writeVector2d(QIODevice *device, const Vector2d &vec); //write a Vector2d

    BFObject();
    //BFObject(BFManager *_manager = 0);
    virtual ~BFObject();
    //create an object
    virtual BFObject *newObject() = 0;
    //duplicate this object
    virtual BFObject *duplicate();

    //draw the object on the QGLWidget
    virtual void draw(QGLWidget *) = 0;
    //set color. This function converts unsigned to 4 doubles and calls setColor(double, double, double, double)
    void setColor(unsigned color);
    //set color. now used by only BFOColoredCircle
    virtual void setColor(double r, double g, double b, double a);
    virtual void setColor(double br);
    //set opacity. Not used.
    virtual void setAlpha(double a);

    //get the Type(Class) of the object. Should return typeid(CLASS).hash_code()
    virtual BFObjectType getType() const = 0;
    //get the shape for intersection detecting
    virtual BFObjectShape getShape() const = 0;
    //get/set parameters of the object
    virtual double getMass() const = 0;
    //The rough radius is the radius of a circle centered at the object's center,
    //large enough to cover the whole object, used for Quadtree
    virtual double getRoughRadius() const = 0;
    virtual Vector2d getPosition() const = 0;
    virtual Vector2d getVelocity() const = 0;
    virtual void setVelocity(Vector2d vel) = 0;
    virtual double getMaxAcceleration() const = 0;
    virtual void setAcceleration(Vector2d acc) = 0;

    //calculate the objects status after time time
    virtual void move(double time) = 0;

    //encode, decode
    //must be called if overwritten by subclasses
    virtual void encode(QIODevice *device);
    virtual void decode(QIODevice *device);

    friend class BFController;
    //BFController *getController();

    //get ID. ID is unique for each object
    BFObjectID getID();
    //comparing ID's
    bool operator <(BFObject &b)
    {
        return id < b.id;
    }

    //property operations. Properties are a map from a std::string to a QVariant, used for storing misc properties
    void setProperty(const std::string &prop, const QVariant &val);
    const QVariant &getProperty(const std::string &prop);
    //void setProperty(const std::string &prop, const std::string &val);
    //overloaded operator for convenience. Not preferred when getting property
    QVariant &operator[](const std::string &prop);

    //apply a ControlEvent to this object, modifying the acceleration and properties
    void applyControlEvent(ControlEvent &ce);

private:
    BFObjectID id;
    //BFManager *manager;
    std::map<std::string, QVariant> properties;
    //BFController *controller;

    friend class BFFactory;
    bool created_from_factory;
    bool deleting_from_factory;
    void setID(BFObjectID newid);

};

#include "../BFManager.h"
#include "../BFController/BFController.h"
#include "../ControlEvent.h"

#endif // BFOBJECT_H
