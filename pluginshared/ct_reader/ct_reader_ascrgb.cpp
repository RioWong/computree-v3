#include "ct_reader_ascrgb.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#include <QDebug>

#include "ct_pointcloud/ct_pointcloudstdvector.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"
#include "ct_coordinates/ct_defaultcoordinatesystem.h"
#include "ct_global/ct_context.h"

#include <limits>

CT_Reader_ASCRGB::CT_Reader_ASCRGB() : CT_AbstractReader()
{
    m_filterRadius = 0;
}

bool CT_Reader_ASCRGB::setFilePath(const QString &filepath)
{
    // Test File validity
    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&f);

            stream.readLine();
            QString line = stream.readLine();
            if (!line.isNull())
            {
                QStringList values = line.split(" ");
                if (values.size() >= 6)
                {
                    f.close();
                    return CT_AbstractReader::setFilePath(filepath);
                }
            }

            f.close();
        }
    }

    return false;
}

void CT_Reader_ASCRGB::setRadiusFilter(const double &radius)
{
    m_filterRadius = radius;
}

CT_AbstractReader* CT_Reader_ASCRGB::copy() const
{
    return new CT_Reader_ASCRGB();
}

void CT_Reader_ASCRGB::protectedInit()
{
    addNewReadableFormat(FileFormat("asc", tr("Fichiers de points ASCII")));
}

void CT_Reader_ASCRGB::protectedCreateOutItemDrawableModelList()
{
    addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_ASCRGB_sceneOut, new CT_Scene(), tr("Scène")));
    addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_ASCRGB_colorOut, new CT_PointsAttributesColor(), tr("Attribut de points (couleurs)")));
}

bool CT_Reader_ASCRGB::protectedReadFile()
{
    // Test File validity
    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            bool filter = m_filterRadius > 0;

            QTextStream stream(&f);

            CT_AbstractUndefinedSizePointCloud* pointCloud = PS_REPOSITORY->createNewUndefinedSizePointCloud();
            CT_ColorCloudStdVector *colorCloud = new CT_ColorCloudStdVector(false);

            double xmin = std::numeric_limits<double>::max();
            double ymin = std::numeric_limits<double>::max();
            double zmin = std::numeric_limits<double>::max();

            double xmax = -std::numeric_limits<double>::max();
            double ymax = -std::numeric_limits<double>::max();
            double zmax = -std::numeric_limits<double>::max();

            QString line;
            bool useOffset = true;
            bool first = true;
            QSharedPointer<CT_AbstractCoordinateSystem> spcs;

            while(!stream.atEnd()
                  && !isStopped())
            {
                line = stream.readLine();
                QStringList values = line.split(" ", QString::SkipEmptyParts);

                if (values.size() > 5)
                {
                    bool okX = false;
                    bool okY = false;
                    bool okZ = false;
                    bool okR = false;
                    bool okG = false;
                    bool okB = false;

                    double x = values.at(0).toDouble(&okX);
                    double y = values.at(1).toDouble(&okY);
                    double z = values.at(2).toDouble(&okZ);

                    if (first && useOffset && okX && okY && okZ)
                    {
                        first = false;
                        if (fabs(x) > 1000 || fabs(y) > 1000 || fabs(z) > 1000)
                        {
                            useOffset = true;
                            spcs = PS_COORDINATES_SYS_MANAGER->registerCoordinateSystem(new CT_DefaultCoordinateSystem(x, y, z));
                        } else {
                            useOffset = false;
                        }
                    }

                    double valueR = values.at(3).toDouble(&okR);
                    double valueG = values.at(4).toDouble(&okG);
                    double valueB = values.at(5).toDouble(&okB);

                    if (valueR < 0) {valueR = 0;}
                    if (valueG < 0) {valueG = 0;}
                    if (valueB < 0) {valueB = 0;}

                    if (valueR > 1) {valueR = 1;}
                    if (valueG > 1) {valueG = 1;}
                    if (valueB > 1) {valueB = 1;}

                    short r = 255*valueR;
                    short g = 255*valueG;
                    short b = 255*valueB;

                    if (okX && okY && okZ && okR && okG && okB)
                    {
                        double distance2D = sqrt(x*x + y*y);

                        if (!filter || (distance2D <= m_filterRadius))
                        {
                            if (x<xmin) {xmin = x;}
                            if (x>xmax) {xmax = x;}
                            if (y<ymin) {ymin = y;}
                            if (y>ymax) {ymax = y;}
                            if (z<zmin) {zmin = z;}
                            if (z>zmax) {zmax = z;}

                            CT_Point &p = pointCloud->addPoint();

                            if (useOffset)
                            {
                                PS_COORDINATES_SYS->convertImport(x, y, z, p(0), p(1), p(2));
                            } else {
                                p(0) = x;
                                p(1) = y;
                                p(2) = z;
                            }

                            CT_Color &color = colorCloud->addColor();

                            color.r = r;
                            color.g = g;
                            color.b = b;
                        }
                    }
                }


                // Ralentit trop la lecture
                //setProgress(stream.pos()*100/fileSize);
            }

            f.close();

            if (colorCloud->size() > 0)
            {
                CT_Repository::CT_AbstractNotModifiablePCIR pcir = PS_REPOSITORY->registerUndefinedSizePointCloud(pointCloud);

                CT_Scene *scene = new CT_Scene(NULL, NULL, pcir);
                scene->setBoundingBox(xmin, ymin, zmin, xmax, ymax, zmax);

                if (useOffset)
                {
                    scene->registerCoordinateSystem(spcs);
                }

                CT_PointsAttributesColor *colors = new CT_PointsAttributesColor(NULL, NULL, pcir, colorCloud);

                addOutItemDrawable(DEF_CT_Reader_ASCRGB_sceneOut, scene);
                addOutItemDrawable(DEF_CT_Reader_ASCRGB_colorOut, colors);

                return true;

            }
            else
            {
                delete pointCloud;
                delete colorCloud;
            }
        }
    }

    return false;
}
