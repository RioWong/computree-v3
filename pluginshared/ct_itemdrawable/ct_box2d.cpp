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

#include "ct_box2d.h"

#include "math.h"

const CT_StandardBox2DDrawManager CT_Box2D::BOX2D_DRAW_MANAGER;

CT_DEFAULT_IA_INIT(CT_Box2D)

CT_Box2D::CT_Box2D() : CT_AbstractAreaShape2D()
{
    setBaseDrawManager(&BOX2D_DRAW_MANAGER);
}

CT_Box2D::CT_Box2D(const CT_OutAbstractSingularItemModel *model,
                     const CT_AbstractResult *result, CT_Box2DData *data) : CT_AbstractAreaShape2D(model, result, data)
{
    setBaseDrawManager(&BOX2D_DRAW_MANAGER);
}

CT_Box2D::CT_Box2D(const QString &modelName,
                     const CT_AbstractResult *result, CT_Box2DData *data) : CT_AbstractAreaShape2D(modelName, result, data)
{
    setBaseDrawManager(&BOX2D_DRAW_MANAGER);
}

QString CT_Box2D::getType() const
{
    return staticGetType();
}

QString CT_Box2D::staticGetType()
{
    return CT_AbstractAreaShape2D::staticGetType() + "/CT_Box2D";
}

float CT_Box2D::getWidth() const
{
    return ((const CT_Box2DData&)getData()).getWidth();
}

float CT_Box2D::getHeight() const
{
    return ((const CT_Box2DData&)getData()).getHeight();
}

CT_AbstractItemDrawable* CT_Box2D::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_Box2D *box = new CT_Box2D((const CT_OutAbstractSingularItemModel *)model, result, (getPointerData() != NULL) ? ((const CT_Box2DData&)getData()).clone() : NULL);
    box->setId(id());
    box->setAlternativeDrawManager(getAlternativeDrawManager());

    return box;
}

