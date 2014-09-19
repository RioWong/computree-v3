/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michal KREBS (ARTS/ENSAM)

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

#ifndef CT_PLANARBSPLINEDATA_H
#define CT_PLANARBSPLINEDATA_H

#include "ct_shapedata.h"

#include <QVector3D>
#include <QVector>

class PLUGINSHAREDSHARED_EXPORT CT_PlanarBSplineData : public CT_ShapeData
{
public:
    CT_PlanarBSplineData();
    CT_PlanarBSplineData(int nCP, int degree);

    void reset(int nCP, int degree);

    void setCP(int index, float x, float y, float z);
    void setCP(int index, const QVector3D &value);
    void setNodalValue(int index, double value);

    const QVector3D &getCPAt(int index) const;
    double getNodalValueAt(int index) const;


    inline int nCP() const {return _nCP;}
    inline int degree() const {return _degree;}

    inline const QVector<QVector3D> &getControlPoints() const {return _controlPoints;}
    inline const QVector<double> &getNodalValues() const {return _nodalSequence;}

    CT_PlanarBSplineData *clone() const;

private:
    int                 _degree;
    int                 _nCP;
    QVector<QVector3D>  _controlPoints;
    QVector<double>     _nodalSequence;
};

#endif // CT_PLANARBSPLINEDATA_H
