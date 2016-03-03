/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
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

#include "ct_abstractareashape2d.h"


CT_AbstractAreaShape2D::CT_AbstractAreaShape2D() : CT_AbstractShape2D()
{
}

CT_AbstractAreaShape2D::CT_AbstractAreaShape2D(const CT_OutAbstractSingularItemModel *model,
                                   const CT_AbstractResult *result,
                                   CT_AreaShape2DData *data) : CT_AbstractShape2D(model, result, data)
{
}

CT_AbstractAreaShape2D::CT_AbstractAreaShape2D(const QString &modelName,
                                   const CT_AbstractResult *result,
                                   CT_AreaShape2DData *data) : CT_AbstractShape2D(modelName, result, data)
{
}

CT_AbstractAreaShape2D::~CT_AbstractAreaShape2D()
{
}

bool CT_AbstractAreaShape2D::contains(double x, double y) const
{
    return getAreaData().contains(x, y);
}

double CT_AbstractAreaShape2D::getArea() const
{
    return getAreaData().getArea();
}

const CT_AreaShape2DData* CT_AbstractAreaShape2D::getPointerAreaData() const
{
    return (CT_AreaShape2DData*) CT_AbstractShape2D::getPointerData();
}

const CT_AreaShape2DData& CT_AbstractAreaShape2D::getAreaData() const
{
    return (CT_AreaShape2DData&) CT_AbstractShape2D::getData();
}

CT_AreaShape2DData* CT_AbstractAreaShape2D::getAreaDataNotConst() const
{
    return (CT_AreaShape2DData*) CT_AbstractShape2D::getDataNotConst();
}
