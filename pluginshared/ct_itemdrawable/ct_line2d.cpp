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

#include "ct_line2d.h"

#include "math.h"

const CT_StandardLine2DDrawManager CT_Line2D::LINE2D_DRAW_MANAGER;

CT_DEFAULT_IA_INIT(CT_Line2D)

CT_Line2D::CT_Line2D() : CT_AbstractShape2D()
{
    setBaseDrawManager(&LINE2D_DRAW_MANAGER);
}

CT_Line2D::CT_Line2D(const CT_OutAbstractSingularItemModel *model,
                     const CT_AbstractResult *result, CT_Line2DData *data) : CT_AbstractShape2D(model, result, data)
{
    setBaseDrawManager(&LINE2D_DRAW_MANAGER);
}

CT_Line2D::CT_Line2D(const QString &modelName,
                     const CT_AbstractResult *result, CT_Line2DData *data) : CT_AbstractShape2D(modelName, result, data)
{
    setBaseDrawManager(&LINE2D_DRAW_MANAGER);
}

QString CT_Line2D::name() const
{
    return tr("3D line");
}

const Eigen::Vector2d &CT_Line2D::getP1() const
{
    return ((const CT_Line2DData&)getData()).getP1();
}

const Eigen::Vector2d &CT_Line2D::getP2() const
{
    return ((const CT_Line2DData&)getData()).getP2();
}

double CT_Line2D::x1() const
{
    return ((const CT_Line2DData&)getData()).x1();
}

double CT_Line2D::y1() const
{
    return ((const CT_Line2DData&)getData()).y1();
}

double CT_Line2D::x2() const
{
     return ((const CT_Line2DData&)getData()).x2();
}

double CT_Line2D::y2() const
{
     return ((const CT_Line2DData&)getData()).y2();
}

double CT_Line2D::length() const
{
     return ((const CT_Line2DData&)getData()).length();
}

CT_AbstractItemDrawable* CT_Line2D::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_Line2D *line = new CT_Line2D((const CT_OutAbstractSingularItemModel *)model, result, (getPointerData() != NULL) ? ((const CT_Line2DData&)getData()).clone() : NULL);
    line->setId(id());
    line->setAlternativeDrawManager(getAlternativeDrawManager());

    return line;
}

