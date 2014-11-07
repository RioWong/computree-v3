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

#include "ct_box2ddata.h"

CT_Box2DData::CT_Box2DData() : CT_AreaShape2DData()
{
}

CT_Box2DData::CT_Box2DData(const QVector2D &center, float height, float width) : CT_AreaShape2DData(center)
{
    _height = height;
    _width = width;        
}

float CT_Box2DData::getHeight() const
{
    return _height;
}

float CT_Box2DData::getWidth() const
{
    return _width;
}

void CT_Box2DData::getBoundingBox(QVector2D &min, QVector2D &max) const
{
    min.setX(_center.x() - _width/2.0);
    min.setY(_center.y() - _height/2.0);
    max.setX(_center.x() + _width/2.0);
    max.setY(_center.y() + _height/2.0);
}

bool CT_Box2DData::contains(float x, float y) const
{
    if (x <= (_center.x() - _width/2.0)) {return false;}
    if (x >= (_center.x() + _width/2.0)) {return false;}

    if (y <= (_center.y() - _height/2.0)) {return false;}
    if (y >= (_center.y() + _height/2.0)) {return false;}

    return true;
}

double CT_Box2DData::getArea() const
{
    return _width*_height;
}

CT_Box2DData* CT_Box2DData::clone() const
{
    return new CT_Box2DData(getCenter(), _height, _width);
}

CT_Box2DData& CT_Box2DData::operator= (const CT_Box2DData& o)
{
    _width = o.getWidth();
    _height = o.getHeight();
    setCenter(o.getCenter());

    return *this;
}
