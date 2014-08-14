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



#ifndef CT_ITEMDRAWABLECONFIGURATION_H
#define CT_ITEMDRAWABLECONFIGURATION_H

#include "interfaces.h"
#include "pluginShared_global.h"

/**
 * @brief Can be used to define painting configuration. Per example if you want to change the points size (use a Double)
 *        or if you want to draw a cube filled or plain (use a Bool), etc....
 */
class PLUGINSHAREDSHARED_EXPORT CT_ItemDrawableConfiguration
{
public:

    /**
     * @brief Type that can be defined and used in GUI
     */
    enum Type
    {
        Bool,
        Int,
        UInt,
        Long,
        Double,
        String,
        Date,
        Time,
        DateTime,
        Url,
        Icon,
        Color,
        StringList
    };

    /**
     * @brief Create a container that can contains multiple configuration
     * @param displayableName : used in GUI. Must not be empty !
     */
    CT_ItemDrawableConfiguration(const QString &displayableName);

    /**
     * @brief Returns a displayable name
     */
    QString name() const;

    /**
     * @brief Returns true if no configuration is present
     */
    bool isEmpty() const;

    /**
     * @brief Returns the number of configuration
     */
    int size() const;

    /**
     * @brief Returns the name of the configuration 'i'
     * @param i : index of the configuration
     */
    QString getVariableName(int i) const;

    /**
     * @brief Returns the value of the configuration 'i'
     * @param i : index of the configuration
     */
    QVariant getVariableValue(int i) const;

    /**
     * @brief Returns the name of the configuration named 'index'
     * @param index : index of the configuration
     */
    QVariant getVariableValue(const QString &index) const;

    /**
     * @brief Returns the type of the configuration 'i'
     * @param i : index of the configuration
     */
    CT_ItemDrawableConfiguration::Type getVariableType(int i) const;

    /**
     * @brief Set the value of the configuration 'i'
     * @param i : index of the configuration
     * @param value : he new value
     */
    bool setVariableValue(int i, const QVariant &value);

    /**
     * @brief Set the value of the configuration name 'index'
     * @param i : index of the configuration
     * @param value : he new value
     */
    bool setVariableValue(const QString &index, const QVariant &value);

    /**
     * @brief Can add all configuration of another container
     * @param other : the other container that have configuration
     * @return false if this object contains already a configuration (compare uniqueName)
     */
    bool addAllConfigurationOf(const CT_ItemDrawableConfiguration &other);

    /**
     * @brief Add a new configuration to this container
     * @param displayableName : a displayable name
     * @param type : the type of the configuration
     * @param default_value : a default value
     * @return normally always true
     */
    bool addNewConfiguration(const QString &displayableName, CT_ItemDrawableConfiguration::Type type, const QVariant &default_value);

    /**
     * @brief Add a new configuration to this container
     * @param uniqueIndex : a unique index
     * @param displayableName : a displayable name
     * @param type : the type of the configuration
     * @param default_value : a default value
     * @return false if uniqueIndex already exist
     */
    bool addNewConfiguration(const QString &uniqueIndex, const QString &displayableName, CT_ItemDrawableConfiguration::Type type, const QVariant &default_value);

    /**
     * @brief Create a configuration
     * @param displayableName : used in GUI. Must not be empty
     * @param nameForComparison : used to search in a list (deprecated)
     */
    Q_DECL_DEPRECATED CT_ItemDrawableConfiguration(const QString &displayableName, const QString &nameForComparison);

    /**
     * @deprecated
     */
    Q_DECL_DEPRECATED QString nameForComparison() const;

protected:

    const QList<QString>& getIndexList() const;
    const QList<QString>& getNameList() const;
    const QList<QVariant>& getValueList() const;
    const QList<CT_ItemDrawableConfiguration::Type>& getTypeList() const;

private:

    QString                                     _name;
    QString                                     _nameForComparison;

    QList<QString>                              _list_config_index;
    QList<QString>                              _list_config_name;
    QList<QVariant>                             _list_config_value;
    QList<CT_ItemDrawableConfiguration::Type>   _list_config_type;
};

#endif // CT_ITEMDRAWABLECONFIGURATION_H
