/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/

#include <QApplication>

#include "view/MainView/gguimanager.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    #include "qtextcodec.h"
#endif

#include <QGLFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGLFormat def = QGLFormat::defaultFormat();

    if(QGLFormat::openGLVersionFlags().testFlag(QGLFormat::OpenGL_Version_3_0)) {
        def.setVersion(3, 0);
        QGLFormat::setDefaultFormat(def);
    }

    CDM_Internationalization language;
    language.loadConfiguration();

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    #endif

    GGuiManager gm(&language);
    gm.initUi();

    GUI_LOG->addInfoMessage(LogInterface::gui, QObject::tr("OpenGL Version used : %1.%2").arg(def.majorVersion()).arg(def.minorVersion()));

    return a.exec();
}
