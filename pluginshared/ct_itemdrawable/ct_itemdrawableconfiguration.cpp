/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël KREBS (ARTS/ENSAM)

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



#include "ct_itemdrawableconfiguration.h"

CT_ItemDrawableConfiguration::CT_ItemDrawableConfiguration(const QString &displayableName) : QObject()
{
    _name = displayableName;
}

CT_ItemDrawableConfiguration::CT_ItemDrawableConfiguration(const CT_ItemDrawableConfiguration &other) : QObject()
{
    _name = other._name;
    _nameForComparison = other._nameForComparison;
    _list_config_index = other._list_config_index;
    _list_config_name = other._list_config_name;
    _list_config_value = other._list_config_value;
    _list_config_type = other._list_config_type;
}

CT_ItemDrawableConfiguration::CT_ItemDrawableConfiguration(const QString &displayableName, const QString &nameForComparison) : QObject()
{
    _name = displayableName;
    _nameForComparison = nameForComparison;
}

QString CT_ItemDrawableConfiguration::name() const
{
    return _name;
}

QString CT_ItemDrawableConfiguration::nameForComparison() const
{
    if(_nameForComparison.isEmpty())
    {
        return name();
    }

    return _nameForComparison;
}

bool CT_ItemDrawableConfiguration::isEmpty() const
{
    return _list_config_name.isEmpty();
}

int CT_ItemDrawableConfiguration::size() const
{
    return _list_config_name.size();
}

QString CT_ItemDrawableConfiguration::getVariableName(int i) const
{
    if((i>=0)
        && (i<size()))
    {
        return _list_config_name.at(i);
    }

    return "";
}

QVariant CT_ItemDrawableConfiguration::getVariableValue(int i) const
{
    if((i>=0)
        && (i<size()))
    {
        return _list_config_value.at(i);
    }

    return "";
}

QVariant CT_ItemDrawableConfiguration::getVariableValue(const QString &index) const
{
    return getVariableValue(_list_config_index.indexOf(index));
}

CT_ItemDrawableConfiguration::Type CT_ItemDrawableConfiguration::getVariableType(int i) const
{
    if((i>=0)
        && (i<size()))
    {
        return _list_config_type.at(i);
    }

    return CT_ItemDrawableConfiguration::String;
}

bool CT_ItemDrawableConfiguration::setVariableValue(int i, const QVariant &value)
{
    if((i>=0)
        && (i<size()))
    {
        _list_config_value.replace(i, value);

        emit valueChanged(i);

        return true;
    }

    return false;
}

bool CT_ItemDrawableConfiguration::setVariableValue(const QString &index, const QVariant &value)
{
    int i = _list_config_index.indexOf(index);
    return setVariableValue(i, value);
}

bool CT_ItemDrawableConfiguration::addAllConfigurationOf(const CT_ItemDrawableConfiguration &other)
{
    QListIterator<QString> it(other.getIndexList());

    while(it.hasNext())
    {
        if(_list_config_index.contains(it.next()))
        {
            return false;
        }
    }

    _list_config_index.append(other.getIndexList());
    _list_config_name.append(other.getNameList());
    _list_config_value.append(other.getValueList());
    _list_config_type.append(other.getTypeList());

    return true;
}

bool CT_ItemDrawableConfiguration::addNewConfiguration(const QString &displayableName, CT_ItemDrawableConfiguration::Type type, const QVariant &default_value)
{
    QString index = QString("%1").arg(_list_config_index.size());

    if(_list_config_index.contains(index))
        return false;

    _list_config_index.append(index);
    _list_config_name.append(displayableName);
    _list_config_value.append(default_value);
    _list_config_type.append(type);

    return true;
}

bool CT_ItemDrawableConfiguration::addNewConfiguration(const QString &uniqueIndex, const QString &displayableName, CT_ItemDrawableConfiguration::Type type, const QVariant &default_value)
{
    if(_list_config_index.contains(uniqueIndex))
        return false;

    _list_config_index.append(uniqueIndex);
    _list_config_name.append(displayableName);
    _list_config_value.append(default_value);
    _list_config_type.append(type);

    return true;
}

//////////// PROTECTED ////////////

const QList<QString>& CT_ItemDrawableConfiguration::getIndexList() const
{
    return _list_config_index;
}

const QList<QString>& CT_ItemDrawableConfiguration::getNameList() const
{
    return _list_config_name;
}

const QList<QVariant>& CT_ItemDrawableConfiguration::getValueList() const
{
    return _list_config_value;
}

const QList<CT_ItemDrawableConfiguration::Type>& CT_ItemDrawableConfiguration::getTypeList() const
{
    return _list_config_type;
}
