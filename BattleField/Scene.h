#ifndef SCENE_H
#define SCENE_H

#include <QGLWidget>

class BattleField;
class BFManager;

//draw all things (including map, blood volume...), apart from the main battlefield
class Scene
{
public:
    Scene(BattleField* bf, BFManager* mg);
    ~Scene();
    void draw();
    void drawMap(double mapSize);
private:
    BattleField* battlefield;
    BFManager* manager;
};

#include "BattleField.h"
#include "BFManager.h"

#endif // SCENE_H
