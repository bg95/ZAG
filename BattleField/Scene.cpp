#include "Scene.h"

Scene::Scene(BattleField *bf, BFManager *mg) :
    battlefield(bf),
    manager(mg)
{
}

Scene::~Scene()
{
}

template<class T>
T min(T x, T y)
{
    return x < y ? x : y;
}

void Scene::draw()
{
    int scaleMap = 3;// scale of 1 / scaleMap compared with the original battlefield

    double windowWidth = battlefield->width(), windowHeight = battlefield->height();
    double windowSize = windowWidth < windowHeight ? windowWidth : windowHeight;
    double width = 0.65 * 2.0 * windowWidth / windowSize;
    double height = 0.65 * 2.0 * windowHeight / windowSize;
    double mapSize = 1.0 / (double)scaleMap * min(height, width);

    glLoadIdentity();
    glTranslatef(width / 2.0 - mapSize / 2.0, height / 2.0 - mapSize / 2.0, -1.01);

    drawMap(mapSize);
}

//this small map will always hang on the right-upper corner, independent of the player's view port
void Scene::drawMap(double mapSize)
{
    glBegin(GL_QUADS);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(- mapSize / 2.0, - mapSize / 2.0, 0.0);
        glVertex3f(mapSize / 2.0, - mapSize / 2.0, 0.0);
        glVertex3f(mapSize / 2.0, mapSize / 2.0, 0.0);
        glVertex3f(- mapSize / 2.0, mapSize / 2.0, 0.0);
        //qDebug("draw map widht size %f", mapSize);
    glEnd();

    double unit = mapSize / 2.0;
    glScalef(unit, unit, unit);
    //hightlight the object controlled by human in the map
    GLfloat mat_emission[4] = { 1.0, 1.0, 1.0, 1.0};
    GLfloat no_mat[4] = { 0.0, 0.0, 0.0, 1.0};
    for(auto iter = manager->getObjects().begin(); iter != manager->getObjects().end(); iter ++) {
        if((*iter) != manager->getObjectControlled())
            (*iter)->draw(battlefield);
        else
        {
            glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
            (*iter)->draw(battlefield);
            glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
        }
    }
}
