/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre Piboule (ONF)

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



#ifndef CT_LineEdit_H
#define CT_LineEdit_H

#include "ct_widgetwithvaluereferenceinterface.h"

#include <QLineEdit>

class PLUGINSHAREDSHARED_EXPORT CT_LineEdit : public CT_WidgetWithValueReferenceInterface
{
    Q_OBJECT

private:

    struct LineEditData
    {
        QString    *_value;
    };

public:
    CT_LineEdit(QString &value, QString description = "");

    QString type() const;
    QList<SettingsNodeGroup*> getAllValues() const;
    bool setAllValues(const QList<SettingsNodeGroup*> &list);

    QWidget* createWidget(QWidget &parent);

    void updateValue();

    bool isValueAndWidgetValueDifferent() const;

    QVariant getValue() const;

    bool setWidgetValue(QVariant val);

protected:

    LineEditData   _data;
    QLineEdit      *_lineEditCreated;
    QString         _description;
};

#endif // CT_LineEdit_H
