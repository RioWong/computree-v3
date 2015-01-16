/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/

#ifndef G3DPAINTER_H
#define G3DPAINTER_H

#include "dm_graphicsviewoptions.h"

#include "ct_point.h"
#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"

#include <QtOpenGL>

// GLU was removed from Qt in version 4.8
#ifdef Q_OS_MAC
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtOpenGL/QGLShaderProgram>
#include <QGLFunctions>
#define QT_GL_SHADERPROGRAM QGLShaderProgram
#define QT_GL_SHADER QGLShader
#define QT_GL_CONTEXT QGLContext
#define QT_GL_FUNCTIONS QGLFunctions
#define QT_GL_INIT_FUNCTIONS initializeGLFunctions
#else
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#define QT_GL_SHADERPROGRAM QOpenGLShaderProgram
#define QT_GL_SHADER QOpenGLShader
#define QT_GL_CONTEXT QOpenGLContext
#define QT_GL_FUNCTIONS QOpenGLFunctions
#define QT_GL_INIT_FUNCTIONS initializeOpenGLFunctions
#endif

class G3DGraphicsView;
template<typename T> class CT_AbstractCloudT;

class G3DPainter : public PainterInterface, public QT_GL_FUNCTIONS
{
    Q_INTERFACES(PainterInterface)

public:

    /**
     * @brief Element in this painter that can be draw with
     *        no call to glBegin multiple times.
     */
    enum MultiDrawableType {
        POINT,              // points
        POINT_FROM_PC,      // points from index in the global point cloud
        LINE,               // lines
        LINE_FROM_PC,       // lines from index in the global point cloud
        TRIANGLE,           // triangles
        TRIANGLE_FROM_PC,   // triangles from index in the global point cloud
        QUAD,               // quads
        QUAD_FROM_PC,       // quads from index in the global point cloud

        OTHER,              // draw only other elements

        DRAW_ALL            // draw all
    };

    G3DPainter();
    virtual ~G3DPainter();

    /**
     * @brief Set the graphics view to access to point/face/edge cloud
     *        color, point/face/edge cloud informations, etc...
     */
    virtual void setGraphicsView(const G3DGraphicsView *gv);
    G3DGraphicsView* graphicsView() const;

    /**
     * @brief Call this functions in your method initializeOpenGL
     */
    virtual void initializeGl();

    /**
     * @brief Begin a new draw (call this functions in beginning of your draw method)
     *        Restore value to default
     */
    virtual void beginNewDraw();

    /**
     * @brief End new draw (call this functions in ending of your draw method)
     *        Do nothing by default
     */
    virtual void endNewDraw();

    /**
     * @brief Call this function to draw only type passed in parameter. This function
     *        optimize call to glBegin to improve the speed of draw for all itemdrawable to draw.
     *
     *        If you want to draw ALL don't call this method.
     *        If you want to draw only element that was not multi drawable call this method with NO_MULTI_AVAILABLE
     */
    virtual void setDrawOnly(G3DPainter::MultiDrawableType type);

    /**
     * @brief Call this method to stop the draw (call the glEnd() function and release the shader).
     * @param releasePointShader : false if you know that you must call glEnd() but without release the point shader because you want
     *                             to draw another point after. (Used by a faked painter to draw with glPushName(...))
     */
    void stopDrawMultiple(bool releasePointShader = true);

    /**
     * @brief Set the fastest increment to use in drawPointCloud method. If 0 use the fastest increment passed in parameter;
     */
    virtual void setPointFastestIncrement(size_t inc);
    size_t pointFastestIncrement() const;

    /**
     * @brief Returns the number of cells of the octree drawed
     */
    int nOctreeCellsDrawed() const;

    /**
     * @brief Set if must draw fastest (with the fastest increment)
     */
    virtual void setDrawFastest(bool enable);
    bool drawFastest() const;

    virtual void save();
    virtual void restore();

    virtual void startRestoreIdentityMatrix(GLdouble *matrix = NULL);
    virtual void stopRestoreIdentityMatrix();

    virtual void enableMultMatrix(bool e);

    virtual void pushMatrix();
    virtual void multMatrix(const Eigen::Matrix4d &matrix);
    virtual void popMatrix();

    virtual void enableSetPointSize(bool enable);
    virtual void setPointSize(float size);
    virtual void setDefaultPointSize(float size);
    virtual void restoreDefaultPointSize();

    virtual void enableSetForcedPointSize(bool enable);

    virtual void setPen(const QPen &pen);
    virtual void restoreDefaultPen();

    virtual void enableSetColor(bool enable);
    virtual void setColor(int r, int g, int b);
    virtual void setColor(QColor color);
    virtual QColor getColor();

    virtual void enableSetForcedColor(bool enable);
    virtual void setForcedColor(int r, int g, int b);
    virtual void setForcedColor(QColor color);

    virtual void setUseColorCloudForPoints(bool enable);
    virtual void setUseNormalCloudForPoints(bool enable);
    virtual void setUseColorCloudForFaces(bool enable);
    virtual void setUseNormalCloudForFaces(bool enable);
    virtual void setUseColorCloudForEdges(bool enable);

    virtual void translate(const double &x, const double &y, const double &z);
    virtual void rotate(const double &alpha, const double &x, const double &y, const double &z);
    virtual void translateThenRotateToDirection(const QVector3D &translation, const QVector3D &direction);
    virtual void scale(const double &x, const double &y, const double &z);

    ///////// GL_POINTS //////////

    virtual void drawPoint(const double &x, const double &y, const double &z);
    virtual void drawPoint(double *p);
    virtual void drawPoint(const size_t &globalIndex);

    virtual void drawPoints(const CT_AbstractMeshModel *mesh);

    virtual void drawPointCloud(const CT_AbstractCloudIndex *pci);

    virtual void drawOctreeOfPoints(const OctreeInterface *octree, DrawOctreeModes modes);

    ///////// GL_QUADS //////////

    virtual void drawCube(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2);
    virtual void drawCube(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2, GLenum faces, GLenum mode );

    virtual void drawQuadFace( const double &x1, const double &y1, const double &z1,
                               const double &x2, const double &y2, const double &z2,
                               const double &x3, const double &y3, const double &z3,
                               const double &x4, const double &y4, const double &z4 );

    virtual void fillQuadFace( const double &x1, const double &y1, const double &z1,
                               const double &x2, const double &y2, const double &z2,
                               const double &x3, const double &y3, const double &z3,
                               const double &x4, const double &y4, const double &z4 );

    virtual void drawQuadFace( const double &x1, const double &y1, const double &z1, int r1, int g1, int b1,
                               const double &x2, const double &y2, const double &z2, int r2, int g2, int b2,
                               const double &x3, const double &y3, const double &z3, int r3, int g3, int b3,
                               const double &x4, const double &y4, const double &z4, int r4, int g4, int b4 );

    virtual void fillQuadFace( const double &x1, const double &y1, const double &z1, int r1, int g1, int b1,
                               const double &x2, const double &y2, const double &z2, int r2, int g2, int b2,
                               const double &x3, const double &y3, const double &z3, int r3, int g3, int b3,
                               const double &x4, const double &y4, const double &z4, int r4, int g4, int b4 );

    virtual void drawRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z);
    virtual void fillRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z);

    virtual void drawRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y);
    virtual void fillRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y);

    virtual void drawRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x);
    virtual void fillRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x);

    ///////// GL_LINES //////////

    virtual void drawLine(const double &x1, const double &y1, const double &z1,
                          const double &x2, const double &y2, const double &z2);

    virtual void drawLine(const size_t &p1GlobalIndex,
                          const size_t &p2GlobalIndex);

    virtual void drawEdges(const CT_AbstractMeshModel *mesh);

    virtual void drawCircle(const double &x, const double &y, const double &z, const double &radius);
    virtual void drawCircle3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double &radius);

    virtual void drawEllipse(const double &x, const double &y, const double &z, const double &radiusA, const double &radiusB);

    virtual void beginPolygon();
    virtual void addPointToPolygon(const double &x, const double &y, const double &z);
    virtual void endPolygon();

    ///////// GL_TRIANGLES //////////

    virtual void drawTriangle(const double &x1, const double &y1, const double &z1,
                              const double &x2, const double &y2, const double &z2,
                              const double &x3, const double &y3, const double &z3);

    virtual void drawTriangle(const size_t &p1GlobalIndex,
                              const size_t &p2GlobalIndex,
                              const size_t &p3GlobalIndex);

    virtual void drawFaces(const CT_AbstractMeshModel *mesh);
    virtual void drawMesh(const CT_AbstractMeshModel *mesh);

    ///////// OTHER //////////

    virtual void drawCylinder(const double &x, const double &y, const double &z, const double &radius, const double &height);
    virtual void drawCylinder3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double &radius, const double &height);

    virtual void drawPyramid(const double &topX, const double &topY, const double &topZ,
                             const double &base1X, const double &base1Y, const double &base1Z,
                             const double &base2X, const double &base2Y, const double &base2Z,
                             const double &base3X, const double &base3Y, const double &base3Z,
                             const double &base4X, const double &base4Y, const double &base4Z);

    virtual void drawPartOfSphere(const double &centerX, const double &centerY, const double &centerZ,
                                  const double &radius,
                                  const double &initTheta, const double &endTheta,
                                  const double &initPhi, const double &endPhi,
                                  bool radians = true );
protected:

    enum GlBeginType {
        GL_BEGIN_POINT,                 // glBegin(GL_POINTS)
        GL_BEGIN_POINT_FROM_PC,         // glBegin(GL_POINTS) but with shader
        GL_BEGIN_LINE,                  // glBegin(GL_LINES)
        GL_BEGIN_LINE_FROM_PC,          // glBegin(GL_LINES) but with shader
        GL_BEGIN_TRIANGLE,              // glBegin(GL_TRIANGLES)
        GL_BEGIN_TRIANGLE_FROM_PC,      // glBegin(GL_TRIANGLES) but with shader
        GL_BEGIN_QUAD,                  // glBegin(GL_QUADS)
        GL_BEGIN_QUAD_FROM_PC,          // glBegin(GL_QUADS) but with shader
        GL_OTHER,                       // other elements
        GL_END_CALLED                   // glEnd() called
    };

    /**
     * @brief Init shader for points if is not already initialized
     */
    void initPointShader();

    /**
     * @brief Bind the point shader
     * @return false bind is ok
     */
    bool bindPointShader(bool force);

    /**
     * @brief Release the point shader
     */
    void releasePointShader(bool bindOk);

private:
    G3DGraphicsView                     *m_gv;                  // Graphics View that used the painter

    G3DPainter::MultiDrawableType       m_drawOnly;             // The type that we must only draw
    G3DPainter::GlBeginType             m_currentGlBeginType;   // The type of the current "glBegin" if "glEnd" was not called. If "glEnd" was called this variable is "GL_END_CALLED".

    CT_AbstractCloudT<CT_Point>         *m_pointCloud;
    CT_AbstractCloudT<CT_Face>          *m_faceCloud;
    CT_AbstractCloudT<CT_Edge>          *m_edgeCloud;

    bool                                m_firstPolygonPointValid;
    Eigen::Vector4d                     m_firstPolygonPoint;

    QT_GL_SHADERPROGRAM                 *m_shaderProgPoint;     // Shader program used for points
    QT_GL_SHADER                        *m_ShaderPoint;
    bool                                m_shaderProgPointError;
    bool                                m_shaderProgPointSet;
    bool                                m_shaderPointError;
    bool                                m_bindShaderPointOK;

    QColor                              _color;                 // Color used in setCurrentColor method
    QColor                              _forcedColor;           // Color used in setCurrentForcedColor method
    GLUquadric                          *_quadric;              // Quadric used to draw cylinder with gluCylinder
    double                              _defaultPointSize;      // Default point size
    bool                                _drawFastest;           // True if we must draw fastest
    bool                                m_usePColorCloud;       // True if we must use color cloud for points
    bool                                m_usePNormalCloud;      // True if we must use normal cloud for points
    bool                                m_useFColorCloud;       // True if we must use color cloud for faces
    bool                                m_useFNormalCloud;      // True if we must use normal cloud for faces
    bool                                m_useEColorCloud;       // True if we must use color cloud for edges

    int                                 _nCallEnableSetColor;       // count how many times the enableSetColor was called
    int                                 _nCallEnableSetForcedColor; // count how many times the enableSetForcedColor was called

    int                                 _nCallEnableSetPointSize;       // count how many times the enableSetPointSize was called
    int                                 _nCallEnableSetForcedPointSize; // count how many times the enableSetForcedPointSize was called

    int                                 _nCallEnablePushMatrix;         // count how many times the enablePushMatrix was called
    GLint                               m_polygonMode;                  // Polygon Mode backup

    size_t                              m_fastestIncrementPoint;        // fastest increment to use if drawFastest() return true
    int                                 m_octreeCellsDraw;              // count how many octree cells was draw

    std::vector< Eigen::Matrix4f >      m_csMatrix;                     // matrix for points in the shader (matrix of coordinate system)
    Eigen::Matrix4d                     m_modelViewMatrix4d;            // model/view matrix of the camera

    static QVector< QPair<double, double> > VECTOR_CIRCLE_FASTEST;
    static const int                        VECTOR_CIRCLE_FASTEST_SIZE = 25;

    static QVector< QPair<double, double> > VECTOR_CIRCLE_NORMAL;
    static const int                        VECTOR_CIRCLE_NORMAL_SIZE = 100;

protected:
    inline CT_AbstractCloudT<CT_Point>* pointCloud() const
    {
        return m_pointCloud;
    }

    inline CT_AbstractCloudT<CT_Edge>* edgeCloud() const
    {
        return m_edgeCloud;
    }

    inline CT_AbstractCloudT<CT_Face>* faceCloud() const
    {
        return m_faceCloud;
    }

    void setCurrentColor();
    void setCurrentForcedColor();

    static QVector< QPair<double, double> > staticInitCircleVector(int size);

    /**
     * @brief Returns true if you can draw the type passed in parameter
     */
    bool canDraw(GlBeginType type) const;

    /**
     * @brief Call this method before draw a type that was in the "GlBeginType" enum. Call to function
     *        glEnd() or glBegin(...) and the bind of the shader was called automatically to begin the draw of your type.
     */
    void startDrawMultiple(GlBeginType type);    

    /**
     * @brief This method call "glEnd()" if the last "glBegin(...)" was not appropriate
     *        for the new type and change the variable "m_currentGlBeginType" to GL_END_CALLED if glEnd() was called.
     *
     *        This method also bind/release the shader in function of the last and new type. Per example if you
     *        use GL_BEGIN_POINT and now you want to use GL_BEGIN_POINT_FROM_PC : the shader is bind but glEnd was not called
     *        because you used and want to use "glBegin(GL_POINTS)".
     *
     * @param newGlBeginType : the new "glBegin" type that you want to call after this method.
     */
    void callGlEndIfGlBeginChanged(GlBeginType newGlBeginType);
};

#endif // G3DPAINTER_H
