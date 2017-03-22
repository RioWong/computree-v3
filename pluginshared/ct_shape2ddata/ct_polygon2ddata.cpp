/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#include "ct_polygon2ddata.h"

#include "ct_triangulation/ct_nodet.h"
#include "ct_triangulation/ct_trianglet.h"
#include "ct_triangulation/ct_edget.h"
#include "ct_iterator/ct_pointiterator.h"

#include <math.h>
#include <limits>
#include <algorithm>

CT_Polygon2DData::CT_Polygon2DData() : CT_AreaShape2DData()
{
    _area = 0.0;
    _areaComputed = false;
}

CT_Polygon2DData::CT_Polygon2DData(const QVector<Eigen::Vector2d *> &vertices, bool copy) : CT_AreaShape2DData()
{
    _area = 0.0;
    _areaComputed = false;

    _min(0) = std::numeric_limits<double>::max();
    _min(1) = std::numeric_limits<double>::max();
    _max(0) = -std::numeric_limits<double>::max();
    _max(1) = -std::numeric_limits<double>::max();


    int size = vertices.size();
    _vertices.resize(size);
    for (int i = 0 ; i < size ; i++)
    {
        Eigen::Vector2d* source = vertices.at(i);
        if (copy)
        {
            _vertices[i] = new Eigen::Vector2d(*source);
        } else {
            _vertices[i] = vertices.at(i);
        }
    }

    // Compute Centroid and affect to center
    computeCentroid();
}

CT_Polygon2DData::CT_Polygon2DData(const QVector<Eigen::Vector3d *> &vertices)
{
    _area = 0.0;
    _areaComputed = false;

    _min(0) = std::numeric_limits<double>::max();
    _min(1) = std::numeric_limits<double>::max();
    _max(0) = -std::numeric_limits<double>::max();
    _max(1) = -std::numeric_limits<double>::max();


    int size = vertices.size();
    _vertices.resize(size);
    for (int i = 0 ; i < size ; i++)
    {
        Eigen::Vector3d* source = vertices.at(i);
        _vertices[i] = new Eigen::Vector2d((*source)(0), (*source)(1));
    }

    // Compute Centroid and affect to center
    computeCentroid();

}

void CT_Polygon2DData::computeCentroid()
{

    double signedArea = 0.0;
    double a = 0.0;

    Eigen::Vector2d ptC;
    ptC(0) = 0;
    ptC(1) = 0;

    _min(0) = std::numeric_limits<double>::max();
    _min(1) = std::numeric_limits<double>::max();
    _max(0) = -std::numeric_limits<double>::max();
    _max(1) = -std::numeric_limits<double>::max();

    for (int i = 0 ; i < _vertices.size() ; i++)
    {
        Eigen::Vector2d *pt = _vertices.at(i);

        if ((*pt)(0) < _min(0)) {_min(0) = (*pt)(0);}
        if ((*pt)(0) > _max(0)) {_max(0) = (*pt)(0);}
        if ((*pt)(1) < _min(1)) {_min(1) = (*pt)(1);}
        if ((*pt)(1) > _max(1)) {_max(1) = (*pt)(1);}
    }


    Eigen::Vector2d *pt1 = NULL;
    Eigen::Vector2d *pt2 = NULL;

    int size = _vertices.size();
    pt1 = _vertices.at(size - 1);

    for (int i = 0 ; i < size ; i++)
    {
        pt2 = _vertices.at(i);

        double x1 = (*pt1)(0) - _min(0);
        double y1 = (*pt1)(1) - _min(1);
        double x2 = (*pt2)(0) - _min(0);
        double y2 = (*pt2)(1) - _min(1);

        a = x1*y2 - x2*y1;
        signedArea += a;

        ptC(0) += (x1 + x2)*a;
        ptC(1) += (y1 + y2)*a;

        pt1 = pt2;
    }


    signedArea = signedArea*0.5;
    ptC(0) = ptC(0) / ((double)6.0*signedArea) + _min(0);
    ptC(1) = ptC(1) / ((double)6.0*signedArea) + _min(1);

    setCenter(ptC);
    _area = signedArea;
    _areaComputed  = true;
}

CT_Polygon2DData::~CT_Polygon2DData()
{
    qDeleteAll(_vertices);
}


CT_Polygon2DData* CT_Polygon2DData::clone() const
{
    return new CT_Polygon2DData(_vertices);
}

void CT_Polygon2DData::getBoundingBox(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    min(0) = _min(0);
    min(1) = _min(1);
    min(2) = 0;
    max(0) = _max(0);
    max(1) = _max(1);
    max(2) = 0;
}

double CT_Polygon2DData::getAreaIfNotSelfIntersecting()
{
    if (!_areaComputed)
    {
        _area = fabs(getSignedArea());
        _areaComputed = true;
    }
    return _area;
}

double CT_Polygon2DData::getSignedArea()
{
    double area = 0;
    Eigen::Vector2d *pt1 = NULL;
    Eigen::Vector2d *pt2 = NULL;

    int size = getVerticesNumber();
    pt1 = _vertices.at(size - 1);
    for (int i = 0 ; i < size ; i++)
    {
        pt2 = _vertices.at(i);

        double x1 = (*pt1)(0) - _min(0);
        double y1 = (*pt1)(1) - _min(1);
        double x2 = (*pt2)(0) - _min(0);
        double y2 = (*pt2)(1) - _min(1);

        area += x1*y2 - x2*y1;
        pt1 = pt2;
    }

    return 0.5*area;
}

double CT_Polygon2DData::getPerimeter() const
{
    double perimeter = 0;
    Eigen::Vector2d *pt1 = NULL;
    Eigen::Vector2d *pt2 = NULL;

    int size = getVerticesNumber();
    pt1 = _vertices.at(size - 1);
    for (int i = 0 ; i < size ; i++)
    {
        pt2 = _vertices.at(i);
        perimeter += sqrt(pow((*pt1)(0) - (*pt2)(0), 2) + pow((*pt1)(1) - (*pt2)(1), 2));
        pt1 = pt2;
    }

    return perimeter;
}

double CT_Polygon2DData::getArea() const
{
    return ((CT_Polygon2DData*) this)->getAreaIfNotSelfIntersecting();
}

bool CT_Polygon2DData::contains(double x, double y) const
{
    if (x < _min(0)) {return false;}
    if (x > _max(0)) {return false;}
    if (y < _min(1)) {return false;}
    if (y > _max(1)) {return false;}

    Eigen::Vector2d *pt1 = NULL;
    Eigen::Vector2d *pt2 = NULL;
    bool result = false;

    int size = getVerticesNumber();
    pt1 = _vertices.at(size - 1);
    for (int i = 0 ; i < size ; i++)
    {
        pt2 = _vertices.at(i);


        if (((((*pt2)(1) <= y) && (y < (*pt1)(1))) || (((*pt1)(1) <= y) && (y < (*pt2)(1)))) &&
                (x < (((*pt1)(0) - (*pt2)(0))*(y - (*pt2)(1))/((*pt1)(1) - (*pt2)(1)) + (*pt2)(0))))
        {
            result = !result;
        }

        pt1 = pt2;
    }
    return result;
}

void CT_Polygon2DData::draw(PainterInterface &painter, bool drawPoints, bool drawLines, double zPlane) const
{
    if(drawPoints || drawLines)
    {
        const QVector<Eigen::Vector2d*>&vertices = getVertices();
        int size = vertices.size();

        Eigen::Vector2d *pt1 = vertices.last();
        for (int i = 0 ; i < size ; i++)
        {
            Eigen::Vector2d *pt2 = vertices.at(i);
            if(drawPoints)
            {
                painter.drawPoint((*pt2)(0), (*pt2)(1), zPlane);
            }
            if(drawLines)
            {
                painter.drawLine((*pt1)(0), (*pt1)(1), zPlane, (*pt2)(0), (*pt2)(1), zPlane);
            }
            pt1 = pt2;
        }
    }
}

CT_Polygon2DData* CT_Polygon2DData::createConvexHull(const CT_PointCloudIndexVector *indices)
{
    // Compute triangulation
    CT_DelaunayT delaunay;

    CT_PointIterator it(indices);
    while(it.hasNext())
    {
        const CT_Point &point = it.next().currentPoint();
        Eigen::Vector3d* pt = new Eigen::Vector3d(point);

        delaunay.insertNode(CT_NodeT::create(pt, NULL, true));
    }

    CT_Polygon2DData* poly = createConvexHull(delaunay);
    return poly;
}

CT_Polygon2DData* CT_Polygon2DData::createConvexHull(const CT_DelaunayT &triangulation)
{
    QVector<Eigen::Vector2d*> convexHull;

    // Retrieve Convex Hull
    QList< QSharedPointer<CT_NodeT> > nodes = triangulation.getHullNodes();

    int size = nodes.size();

    if (size < 3) {return NULL;}

    for (int i = 0 ; i < size ; i++)
    {
        QSharedPointer<CT_NodeT> node = nodes.at(i);
        Eigen::Vector3d* point = node.data()->getPoint();

        Eigen::Vector2d *vertice = new Eigen::Vector2d((*point)(0), (*point)(1));
        convexHull.append(vertice);
    }

    return new CT_Polygon2DData(convexHull);
}



void CT_Polygon2DData::orderPointsByXY(QList<Eigen::Vector2d*> &pointList)
{
    std::sort(pointList.begin(), pointList.end(), compareV2d);
}

bool CT_Polygon2DData::compareV2d(const Eigen::Vector2d* p1, const Eigen::Vector2d* p2)
{
     return ((*p1)(0) < (*p2)(0) || ((*p1)(0) == (*p2)(0) && (*p1)(1) < (*p2)(1)));
}

// 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
// Returns a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
double CT_Polygon2DData::cross(const Eigen::Vector2d* O, const Eigen::Vector2d* A, const Eigen::Vector2d* B)
{
    return ((*A)(0) - (*O)(0)) * ((*B)(1) - (*O)(1)) - ((*A)(1) - (*O)(1)) * ((*B)(0) - (*O)(0));
}

// Returns a list of points on the convex hull in counter-clockwise order.
// Note: the last point in the returned list is the same as the first one.
CT_Polygon2DData* CT_Polygon2DData::createConvexHull(QList<Eigen::Vector2d*> &orderedCandidates)
{
    // Adapted from http://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
    // And also: http://www.codecodex.com/wiki/Andrew's_Monotone_Chain_Algorithm

    int n = orderedCandidates.size();

    if (n < 3) {return NULL;}

    int k = 0;

    QVector<Eigen::Vector2d*> H(2*n);

    // Build lower hull
    for (int i = 0 ; i < n ; ++i) {
        while (k >= 2 && cross(H[k-2], H[k-1], orderedCandidates[i]) <= 0) {k--;}
        H[k++] = orderedCandidates[i];
    }

    // Build upper hull
    for (int i = n-2, t = k + 1 ; i >= 0; i--) {
        while (k >= t && cross(H[k-2], H[k-1], orderedCandidates[i]) <= 0) {k--;}
        H[k++] = orderedCandidates[i];
    }

    H.resize(k-1);

    return new CT_Polygon2DData(H);
}

void CT_Polygon2DData::orderPointsByXY(QList<Eigen::Vector3d*> &pointList)
{
    std::sort(pointList.begin(), pointList.end(), compareV3d);
}

bool CT_Polygon2DData::compareV3d(const Eigen::Vector3d* p1, const Eigen::Vector3d* p2)
{
     return ((*p1)(0) < (*p2)(0) || ((*p1)(0) == (*p2)(0) && (*p1)(1) < (*p2)(1)));
}

// 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
// Returns a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
double CT_Polygon2DData::cross(const Eigen::Vector3d* O, const Eigen::Vector3d* A, const Eigen::Vector3d* B)
{
    return ((*A)(0) - (*O)(0)) * ((*B)(1) - (*O)(1)) - ((*A)(1) - (*O)(1)) * ((*B)(0) - (*O)(0));
}

// Returns a list of points on the convex hull in counter-clockwise order.
// Note: the last point in the returned list is the same as the first one.
CT_Polygon2DData* CT_Polygon2DData::createConvexHull(QList<Eigen::Vector3d*> &orderedCandidates)
{
    // Adapted from http://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
    // And also: http://www.codecodex.com/wiki/Andrew's_Monotone_Chain_Algorithm

    int n = orderedCandidates.size();

    if (n < 3) {return NULL;}

    int k = 0;

    QVector<Eigen::Vector3d*> H(2*n);

    // Build lower hull
    for (int i = 0 ; i < n ; ++i) {
        while (k >= 2 && cross(H[k-2], H[k-1], orderedCandidates[i]) <= 0) {k--;}
        H[k++] = orderedCandidates[i];
    }

    // Build upper hull
    for (int i = n-2, t = k + 1; i >= 0; i--) {
        while (k >= t && cross(H[k-2], H[k-1], orderedCandidates[i]) <= 0) {k--;}
        H[k++] = orderedCandidates[i];
    }

    H.resize(k-1);


    return  new CT_Polygon2DData(H);

}


