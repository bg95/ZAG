#ifndef SCENE_H
#define SCENE_H

#include <QGLWidget>

class BattleField;
class BFManager;

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
