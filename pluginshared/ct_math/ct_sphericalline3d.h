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

#ifndef CT_SPHERICALLINE3D_H
#define CT_SPHERICALLINE3D_H

#include "pluginShared_global.h"
#include "ct_mathpoint.h"
#include "ct_shapedata/ct_linedata.h"

template<typename xyzPoint>
class CT_SphericalLine3D
{
public:

    CT_SphericalLine3D(xyzPoint* p1, xyzPoint* p2, bool orderByZ)
    {
        if (orderByZ && (CT_MathPoint::pZ<xyzPoint>(*p1) > CT_MathPoint::pZ<xyzPoint>(*p2)))
        {
            _p1 = p2;
            _p2 = p1;
        } else {
            _p1 = p1;
            _p2 = p2;
        }

        CT_SphericalLine3D::convertToSphericalCoordinates(_p1, _p2, _phi, _theta, _length);
    }


    inline CT_LineData* getLineData()
    {
        return new CT_LineData(QVector3D(CT_MathPoint::pX<xyzPoint>(*_p1), CT_MathPoint::pY<xyzPoint>(*_p1), CT_MathPoint::pZ<xyzPoint>(*_p1)), QVector3D(CT_MathPoint::pX<xyzPoint>(*_p2), CT_MathPoint::pY<xyzPoint>(*_p2), CT_MathPoint::pZ<xyzPoint>(*_p2)));
    }

    // champs
    xyzPoint*   _p1;        /*!< Point le plus bas */
    xyzPoint*   _p2;        /*!< Point le plus haut*/
    float       _phi;       /*!< Angle zénithal (du haut vers le bas)*/
    float       _theta;     /*!< Angle azimuthal (en sens anti-horaire)*/
    float       _length;    /*!< Longueur de segment*/



    // Méthodes statiques

    inline static QList<CT_SphericalLine3D*> getLinesFromPoints(const QList<xyzPoint*> &points, float minZenithalAngle = 0, float maxZenithalAngle = M_PI_2, bool orderByZ = true)
    {
        QList<CT_SphericalLine3D*> liste;
        int size = points.size();

        for (int i = 0 ; i < size ; i++)
        {
            xyzPoint* pi = points.at(i);
            for (int j = i+1 ; j < size ; j++)
            {
                xyzPoint* pj = points.at(j);
                CT_SphericalLine3D* line = new CT_SphericalLine3D(pi, pj, orderByZ);
                if ((line->_phi >= minZenithalAngle) && (line->_phi <= maxZenithalAngle))
                {
                    liste.append(line);
                } else {
                    delete line;
                }
            }
        }

        return liste;
    }

    inline static void convertToSphericalCoordinates(const xyzPoint* p1, const xyzPoint* p2, float &phi, float &theta, float &length)
    {
        float x = (CT_MathPoint::pX<xyzPoint>(*p2) - CT_MathPoint::pX<xyzPoint>(*p1));
        float y = (CT_MathPoint::pY<xyzPoint>(*p2) - CT_MathPoint::pY<xyzPoint>(*p1));
        float z = (CT_MathPoint::pZ<xyzPoint>(*p2) - CT_MathPoint::pZ<xyzPoint>(*p1));

        convertToSphericalCoordinates(x, y, z, phi, theta, length);
    }

    inline static void convertToSphericalCoordinates(float x, float y, float z, float &phi, float &theta, float &length)
    {
        length = sqrt(x*x + y*y + z*z);
        phi = acos(z/length);
        theta = acos(x/sqrt(x*x + y*y));
        if (y < 0)
        {
           theta = 2*M_PI - theta;
        }
    }

    inline static void convertToCartesianCoordinates(float phi, float theta, float length, float &x, float &y, float &z)
    {
        x = length * cos(theta) * sin(phi);
        y = length * sin(theta) * sin(phi);
        z = length * cos(phi);
    }
};

#endif // CT_SPHERICALLINE3D_H
