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

#include "ct_datasourcegeo.h"


CT_DataSourceGeo::CT_DataSourceGeo() : CT_DataSource()
{
}

CT_DataSourceGeo::CT_DataSourceGeo(const CT_OutAbstractSingularItemModel *model,
                                   const CT_AbstractResult *result) : CT_DataSource(model, result)
{
}

CT_DataSourceGeo::CT_DataSourceGeo(const QString &modelName,
                                   const CT_AbstractResult *result) : CT_DataSource(modelName, result)
{
}

CT_DataSourceGeo::~CT_DataSourceGeo()
{
}

QString CT_DataSourceGeo::getType() const
{
    return staticGetType();
}

QString CT_DataSourceGeo::staticGetType()
{
    return CT_DataSource::staticGetType() + "/CT_DataSourceGeo";
}

bool CT_DataSourceGeo::addReader(CT_AbstractReader *reader)
{
    if (CT_DataSource::addReader(reader))
    {
        if (reader->getHeader()->hasBoundingBox())
        {
            Eigen::Vector3d min, max;
            reader->getHeader()->getBoundingBox(min, max);

            if (min(0) < _minCoordinates(0)) {_minCoordinates(0) = min(0);}
            if (min(1) < _minCoordinates(1)) {_minCoordinates(1) = min(1);}
            if (min(2) < _minCoordinates(2)) {_minCoordinates(2) = min(2);}

            if (max(0) > _maxCoordinates(0)) {_maxCoordinates(0) = max(0);}
            if (max(1) > _maxCoordinates(1)) {_maxCoordinates(1) = max(1);}
            if (max(2) > _maxCoordinates(2)) {_maxCoordinates(2) = max(2);}

            return true;
        } else {
            PS_LOG->addMessage(LogInterface::info, LogInterface::reader, tr("Impossible d'ajouter un reader sans BoundingBox à une CT_DataSourceGeo"));
        }
    }

    updateCenterFromBoundingBox();

    return false;
}

QList<QSharedPointer<CT_AbstractReader> > CT_DataSourceGeo::getReadersIntersecting(const CT_Shape2DData &data) const
{
    Eigen::Vector3d min, max;
    data.getBoundingBox(min, max);

    return getReadersIntersecting(min, max);
}


QList<QSharedPointer<CT_AbstractReader> > CT_DataSourceGeo::getReadersIntersecting(const Eigen::Vector3d& min,const Eigen::Vector3d& max) const
{
    QList<QSharedPointer<CT_AbstractReader> > list;

    if (min(0) > _maxCoordinates(0)) {return list;}
    if (min(1) > _maxCoordinates(1)) {return list;}
    if (max(0) < _minCoordinates(0)) {return list;}
    if (max(1) < _minCoordinates(1)) {return list;}

    QListIterator<QSharedPointer<CT_AbstractReader> > it(_readers);

    while (it.hasNext())
    {
        QSharedPointer<CT_AbstractReader> reader = it.next();

        Eigen::Vector3d minR, maxR;
        reader->getHeader()->getBoundingBox(minR, maxR);

        bool intersection = true;

        if (min(0) > maxR(0)) {intersection = false;}
        if (min(1) > maxR(1)) {intersection = false;}
        if (max(0) < minR(0)) {intersection = false;}
        if (max(1) < minR(1)) {intersection = false;}

        if (intersection) {list.append(reader);}
    }

    return list;
}

bool CT_DataSourceGeo::intersects(const CT_Shape2DData &data)
{
    Eigen::Vector3d min, max;
    data.getBoundingBox(min, max);
    if (min(0) > _maxCoordinates(0)) {return false;}
    if (min(1) > _maxCoordinates(1)) {return false;}
    if (max(0) < _minCoordinates(0)) {return false;}
    if (max(1) < _minCoordinates(1)) {return false;}

    return true;
}

CT_AbstractItemDrawable *CT_DataSourceGeo::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_DataSourceGeo *cpy = new CT_DataSourceGeo((const CT_OutAbstractSingularItemModel*)model, result);
    cpy->_readers.append(_readers);
    cpy->_lastReaderIndice = _lastReaderIndice;
    cpy->_minCoordinates = _minCoordinates;
    cpy->_maxCoordinates = _maxCoordinates;

    cpy->updateCenterFromBoundingBox();

    return cpy;
}

