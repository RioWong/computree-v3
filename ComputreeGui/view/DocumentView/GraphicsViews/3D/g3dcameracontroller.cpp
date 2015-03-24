#include "g3dcameracontroller.h"
#include "manipulatedCameraFrame.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <limits>

#include "dm_guimanager.h"

#include "ct_global/ct_context.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_cloudindex/registered/abstract/ct_abstractmodifiablecloudindexregistered.h"
#include "ct_cloudindex/abstract/ct_abstractmodifiablecloudindex.h"

#include "ct_iterator/ct_pointiterator.h"

#include "view/DocumentView/GraphicsViews/3D/g3dcamera.h"

G3DCameraController::G3DCameraController() : DM_GraphicsViewCamera()
{
    _realCamera = NULL;
    _view = NULL;
}

void G3DCameraController::setRealCamera(const qglviewer::Camera *camera)
{
    if(_realCamera != NULL)
        disconnect(_realCamera->frame(), NULL, this, NULL);

    _realCamera = (qglviewer::Camera*)camera;

    if(_realCamera != NULL)
        connect(_realCamera->frame(), SIGNAL(manipulated()), this, SIGNAL(coordinatesChanged()), Qt::DirectConnection);
}

void G3DCameraController::setView(const DM_GraphicsView *view)
{
    _view = (DM_GraphicsView*)view;
}

void G3DCameraController::setLastItemSelectedCameraCenter(double x, double y, double z)
{
    _lastItemSelectedCenter.setValue(x, y, z);
}

double G3DCameraController::x() const
{
    if(_realCamera != NULL)
        return _realCamera->position().x;

    return 0;
}
double G3DCameraController::y() const
{
    if(_realCamera != NULL)
        return _realCamera->position().y;

    return 0;
}

double G3DCameraController::z() const
{
    if(_realCamera != NULL)
        return _realCamera->position().z;

    return 0;
}

double G3DCameraController::cx() const
{
    if(_realCamera != NULL)
        return _realCamera->sceneCenter().x;

    return 0;
}

double G3DCameraController::cy() const
{
    if(_realCamera != NULL)
        return _realCamera->sceneCenter().y;

    return 0;
}

double G3DCameraController::cz() const
{
    if(_realCamera != NULL)
        return _realCamera->sceneCenter().z;

    return 0;
}

double G3DCameraController::rx() const
{
    if(_realCamera != NULL)
        return _realCamera->viewDirection().x;

    return 0;
}

double G3DCameraController::ry() const
{
    if(_realCamera != NULL)
        return _realCamera->viewDirection().y;

    return 0;
}

double G3DCameraController::rz() const
{
    if(_realCamera != NULL)
        return _realCamera->viewDirection().z;

    return 0;
}

float G3DCameraController::focusDistance() const
{
    if(_realCamera != NULL)
        return _realCamera->focusDistance();

    return 0;
}

CameraInterface::CameraType G3DCameraController::type() const
{
    if(_realCamera != NULL)
        return CameraInterface::PERSPECTIVE;

    return (_realCamera->type() == qglviewer::Camera::PERSPECTIVE) ? CameraInterface::PERSPECTIVE : CameraInterface::ORTHOGRAPHIC;
}

const GLdouble* G3DCameraController::orientationMatrix() const
{
    return _realCamera->orientation().matrix();
}

const GLdouble* G3DCameraController::orientationMatrixInversed() const
{
    return _realCamera->orientation().inverse().matrix();
}

void G3DCameraController::modelViewMatrix(GLdouble m[16]) const
{
    _realCamera->getModelViewMatrix(m);
}

void G3DCameraController::getOrientation(double &q0, double &q1, double &q2, double &q3) const
{
    if(_realCamera == NULL)
        return;

    _realCamera->frame()->getOrientation(q0, q1, q2, q3);
}

bool G3DCameraController::getCameraFrustumPlanesCoefficients(GLdouble coef[6][4]) const
{
    if(_realCamera == NULL)
        return false;

    _realCamera->getFrustumPlanesCoefficients(coef);
    return true;
}

Eigen::Vector3d G3DCameraController::projectedCoordinatesOf(const Eigen::Vector3d &src) const
{
    Eigen::Vector3d ret;

    if(_realCamera == NULL)
        return ret;

    qglviewer::Vec v = _realCamera->projectedCoordinatesOf(qglviewer::Vec(src(0), src(1), src(2)));
    ret(0) = v.x;
    ret(1) = v.y;
    ret(2) = v.z;

    return ret;
}

Eigen::Vector3d G3DCameraController::openGLProjectedCoordinatesOf(const Eigen::Vector3d &src) const
{
    GLint Viewport[4];
    GLdouble Projection[16], Modelview[16];
    GLdouble matrix[16];
    // Precomputation begin
    glGetIntegerv(GL_VIEWPORT , Viewport);
    glGetDoublev (GL_MODELVIEW_MATRIX , Modelview);
    glGetDoublev (GL_PROJECTION_MATRIX, Projection);
    for (unsigned short m=0; m<4; ++m)
    {
        for (unsigned short l=0; l<4; ++l)
        {
            double sum = 0.0;
            for (unsigned short k=0; k<4; ++k)
                sum += Projection[l+4*k]*Modelview[k+4*m];
            matrix[l+4*m] = sum;
        }
    }
    // Precomputation end
    GLdouble v[4], vs[4];
    v[0]=src.x(); v[1]=src.y(); v[2]=src.z(); v[3]=1.0;
    vs[0]=matrix[0 ]*v[0] + matrix[4 ]*v[1] + matrix[8 ]*v[2] + matrix[12 ]*v[3];
    vs[1]=matrix[1 ]*v[0] + matrix[5 ]*v[1] + matrix[9 ]*v[2] + matrix[13 ]*v[3];
    vs[2]=matrix[2 ]*v[0] + matrix[6 ]*v[1] + matrix[10]*v[2] + matrix[14 ]*v[3];
    vs[3]=matrix[3 ]*v[0] + matrix[7 ]*v[1] + matrix[11]*v[2] + matrix[15 ]*v[3];
    vs[0] /= vs[3];
    vs[1] /= vs[3];
    vs[2] /= vs[3];
    vs[0] = vs[0] * 0.5 + 0.5;
    vs[1] = vs[1] * 0.5 + 0.5;
    vs[2] = vs[2] * 0.5 + 0.5;
    vs[0] = vs[0] * Viewport[2] + Viewport[0];
    vs[1] = vs[1] * Viewport[3] + Viewport[1];

    return Eigen::Vector3d(vs[0], Viewport[3]-vs[1], vs[2]);
}

Eigen::Vector3d G3DCameraController::unprojectedCoordinatesOf(const Eigen::Vector3d &src) const
{
    Eigen::Vector3d ret;

    if(_realCamera == NULL)
        return ret;

    qglviewer::Vec v = _realCamera->unprojectedCoordinatesOf(qglviewer::Vec(src.x(), src.y(), src.z()));
    ret(0) = v.x;
    ret(1) = v.y;
    ret(2) = v.z;

    return ret;
}

Eigen::Vector3d G3DCameraController::upVector() const
{
    Eigen::Vector3d ret;

    if(_realCamera == NULL)
        return ret;

    qglviewer::Vec v = _realCamera->upVector();
    ret(0) = v.x;
    ret(1) = v.y;
    ret(2) = v.z;

    return ret;
}

Eigen::Vector3d G3DCameraController::rightVector() const
{
    Eigen::Vector3d ret;

    if(_realCamera == NULL)
        return ret;

    qglviewer::Vec v = _realCamera->rightVector();
    ret(0) = v.x;
    ret(1) = v.y;
    ret(2) = v.z;

    return ret;
}

////////////// SLOTS ////////////////

void G3DCameraController::setX(double x)
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos = _realCamera->position();
    pos.x = x;

    _realCamera->setPosition(pos);
    redrawTheView();
}

void G3DCameraController::setY(double y)
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos = _realCamera->position();
    pos.y = y;

    _realCamera->setPosition(pos);
    redrawTheView();
}

void G3DCameraController::setZ(double z)
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos = _realCamera->position();
    pos.z = z;

    _realCamera->setPosition(pos);
    redrawTheView();
}

void G3DCameraController::setCX(double cx)
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos = _realCamera->sceneCenter();
    pos.x = cx;

    _realCamera->setSceneCenter(pos);
    redrawTheView();
}

void G3DCameraController::setCY(double cy)
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos = _realCamera->sceneCenter();
    pos.y = cy;

    _realCamera->setSceneCenter(pos);
    redrawTheView();
}

void G3DCameraController::setCZ(double cz)
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos = _realCamera->sceneCenter();
    pos.z = cz;

    _realCamera->setSceneCenter(pos);
    redrawTheView();
}

void G3DCameraController::setRX(double xRot)
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos = _realCamera->viewDirection();
    pos.x = xRot;

    _realCamera->setViewDirection(pos);
    redrawTheView();
}

void G3DCameraController::setRY(double yRot)
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos = _realCamera->viewDirection();
    pos.y = yRot;

    _realCamera->setViewDirection(pos);
    redrawTheView();
}

void G3DCameraController::setRZ(double zRot)
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos = _realCamera->viewDirection();
    pos.z = zRot;

    _realCamera->setViewDirection(pos);
    redrawTheView();
}

void G3DCameraController::setType(CameraInterface::CameraType type)
{
    if(_realCamera == NULL)
        return;

    if(type == CameraInterface::PERSPECTIVE)
        _realCamera->setType(qglviewer::Camera::PERSPECTIVE);
    else
        _realCamera->setType(qglviewer::Camera::ORTHOGRAPHIC);
}

void G3DCameraController::setPosition(double x, double y, double z, bool redrawView)
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;

    _realCamera->setPosition(pos);

    if(redrawView)
        redrawTheView();
}

void G3DCameraController::setSceneCenter(double cx, double cy, double cz, bool redrawView)
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos;
    pos.x = cx;
    pos.y = cy;
    pos.z = cz;

    _realCamera->setSceneCenter(pos);

    if(redrawView)
        redrawTheView();
}

void G3DCameraController::setViewDirection(double rx, double ry, double rz, bool redrawView)
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos;
    pos.x = rx;
    pos.y = ry;
    pos.z = rz;

    _realCamera->setViewDirection(pos);

    if(redrawView)
        redrawTheView();
}

void G3DCameraController::setOrientation(double q0, double q1, double q2, double q3, bool redrawView)
{
    if(_realCamera == NULL)
        return;

    _realCamera->frame()->setOrientation(q0, q1, q2, q3);

    if(redrawView)
        redrawTheView();
}

void G3DCameraController::showEntireScene()
{
    if(_realCamera == NULL)
        return;

    _realCamera->showEntireScene();
    redrawTheView();
}

void G3DCameraController::alignCameraToInvXAxis()
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos = _realCamera->position();

    double max = pos.x;

    if(fabs(max) < fabs(pos.y))
    {
        max = pos.y;
    }

    if(fabs(max) < fabs(pos.z))
    {
        max = pos.z;
    }

    pos.x = -fabs(max);
    pos.y = 0;
    pos.z = 0;

    _realCamera->setOrientation(-M_PI_2, M_PI_2);
    _realCamera->setPosition(pos);

    redrawTheView();
}

void G3DCameraController::alignCameraToInvYAxis()
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos = _realCamera->position();

    double max = pos.x;

    if(fabs(max) < fabs(pos.y))
    {
        max = pos.y;
    }

    if(fabs(max) < fabs(pos.z))
    {
        max = pos.z;
    }

    pos.x = 0;
    pos.y = -fabs(max);
    pos.z = 0;

    _realCamera->setOrientation(0, -M_PI_2);
    _realCamera->setPosition(pos);

    redrawTheView();
}

void G3DCameraController::alignCameraToInvZAxis()
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos = _realCamera->position();

    double max = pos.x;

    if(fabs(max) < fabs(pos.y))
    {
        max = pos.y;
    }

    if(fabs(max) < fabs(pos.z))
    {
        max = pos.z;
    }

    pos.x = 0;
    pos.y = 0;
    pos.z = -fabs(max);

    _realCamera->setOrientation(M_PI, 0);
    _realCamera->setPosition(pos);

    redrawTheView();
}

void G3DCameraController::alignCameraToXAxis()
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos = _realCamera->position();

    double max = pos.x;

    if(fabs(max) < fabs(pos.y))
    {
        max = pos.y;
    }

    if(fabs(max) < fabs(pos.z))
    {
        max = pos.z;
    }

    pos.x = fabs(max);
    pos.y = 0;
    pos.z = 0;

    _realCamera->setOrientation(M_PI_2, M_PI_2);
    _realCamera->setPosition(pos);

    redrawTheView();
}

void G3DCameraController::alignCameraToYAxis()
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos = _realCamera->position();

    double max = pos.x;

    if(fabs(max) < fabs(pos.y))
    {
        max = pos.y;
    }

    if(fabs(max) < fabs(pos.z))
    {
        max = pos.z;
    }

    pos.x = 0;
    pos.y = fabs(max);
    pos.z = 0;

    _realCamera->setOrientation(M_PI, -M_PI_2);
    _realCamera->setPosition(pos);

    redrawTheView();
}

void G3DCameraController::alignCameraToZAxis()
{
    if(_realCamera == NULL)
        return;

    qglviewer::Vec pos = _realCamera->position();
    double max = pos.x;

    if(fabs(max) < fabs(pos.y))
    {
        max = pos.y;
    }

    if(fabs(max) < fabs(pos.z))
    {
        max = pos.z;
    }

    pos.x = 0;
    pos.y = 0;
    pos.z = fabs(max);

    _realCamera->setOrientation(0, 0);
    _realCamera->setPosition(pos);

    redrawTheView();
}

void G3DCameraController::initCameraCenter()
{
    if(_realCamera == NULL)
        return;

    _realCamera->setSceneCenter(qglviewer::Vec(0,0,0));
    _realCamera->centerScene();

    redrawTheView();
}

void G3DCameraController::fixCameraCenterToItemsBarycenter()
{
    if((_view == NULL)
            || (_realCamera == NULL))
        return;

    QListIterator<CT_AbstractItemDrawable*> it(_view->getDocumentView().getItemDrawable());

    double bx = 0, by = 0, bz = 0;
    int size = 0;

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        bx += item->getCenterX();
        by += item->getCenterY();
        bz += item->getCenterZ();

        ++size;
    }

    qglviewer::Vec vec;
    vec.setValue(0, 0, 0);

    if(size != 0)
        vec.setValue(bx/((double)size), by/((double)size), bz/((double)size));

    _realCamera->setSceneCenter(vec);
    _realCamera->centerScene();

    redrawTheView();
}

void G3DCameraController::fixCameraCenterToSelectedItemsBarycenter()
{
    if((_view == NULL)
            || (_realCamera == NULL))
        return;

    QList<CT_AbstractItemDrawable*> selected = _view->getDocumentView().getSelectedItemDrawable();
    QListIterator<CT_AbstractItemDrawable*> it(selected);

    double bx = 0, by = 0, bz = 0;

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        bx += item->getCenterX();
        by += item->getCenterY();
        bz += item->getCenterZ();
    }

    double size = selected.size();

    CT_SPCIR selec = _view->getSelectedPoints();

    CT_PointIterator itP(selec);

    if(itP.hasNext())
    {
        size_t sizeP = itP.size();
        size_t increment = log10(sizeP);

        if(increment < 3)
            increment = 1;
        else
            increment = pow(10, increment)/10;

        while(itP.hasNext())
        {
            itP.next();

            const CT_Point &point = itP.currentPoint();

            bx += point(0);
            by += point(1);
            bz += point(2);

            ++size;
            itP.jump(increment);
        }
    }

    qglviewer::Vec vec;
    vec.setValue(0, 0, 0);

    if(size != 0)
        vec.setValue(bx/size, by/size, bz/size);

    _realCamera->setSceneCenter(vec);
    _realCamera->centerScene();

    redrawTheView();
}

void G3DCameraController::fitCameraToVisibleItems()
{
    if((_view == NULL) || (_realCamera == NULL)) {return;}

    QList<CT_AbstractItemDrawable*> items = _view->getDocumentView().getItemDrawable();
    QListIterator<CT_AbstractItemDrawable*> it(items);

    qglviewer::Vec min;
    qglviewer::Vec max;

    Eigen::Vector3d minBB;
    Eigen::Vector3d maxBB;

    min.x = std::numeric_limits<double>::max();
    min.y = std::numeric_limits<double>::max();
    min.z = std::numeric_limits<double>::max();

    max.x = -std::numeric_limits<double>::max();
    max.y = -std::numeric_limits<double>::max();
    max.z = -std::numeric_limits<double>::max();

    /*min.x = 1000000;
    min.y = 1000000;
    min.z = 1000000;

    max.x = 1000000.1;
    max.y = 1000000.1;
    max.z = 1000000.1;*/

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if (item->hasBoundingBox())
        {
            item->getBoundingBox(minBB, maxBB);

            if (minBB(0) < min.x) {min.x = minBB(0);}
            if (minBB(1) < min.y) {min.y = minBB(1);}
            if (minBB(2) < min.z) {min.z = minBB(2);}

            if (maxBB(0) > max.x) {max.x = maxBB(0);}
            if (maxBB(1) > max.y) {max.y = maxBB(1);}
            if (maxBB(2) > max.z) {max.z = maxBB(2);}

            // If boundingBox not defined : take center
            if (item->getCenterX() < min.x) {min.x = item->getCenterX();}
            if (item->getCenterY() < min.y) {min.y = item->getCenterY();}
            if (item->getCenterZ() < min.z) {min.z = item->getCenterZ();}

            if (item->getCenterX() > max.x) {max.x = item->getCenterX();}
            if (item->getCenterY() > max.y) {max.y = item->getCenterY();}
            if (item->getCenterZ() > max.z) {max.z = item->getCenterZ();}
        }
    }

    if ((min.x < std::numeric_limits<double>::max()) &&
            (min.y < std::numeric_limits<double>::max()) &&
            (min.z < std::numeric_limits<double>::max()) &&
            (max.x > -std::numeric_limits<double>::max()) &&
            (max.y > -std::numeric_limits<double>::max()) &&
            (max.z > -std::numeric_limits<double>::max()))
    {
        //TODO : soutraire l'offset avant de passer à la caméra

        ((G3DCamera*)_realCamera)->setBoundingBox(min, max);
        _realCamera->fitBoundingBox(min, max);
        redrawTheView();
    }
}

void G3DCameraController::redrawTheView()
{
    if(_view != NULL)
        _view->redraw();
}

