#include "ct_reader_xyb.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#include "ct_itemdrawable/ct_pointsattributesscalartemplated.h"
#include "ct_global/ct_context.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"
#include "ct_iterator/ct_mutablepointiterator.h"

#include <limits>

CT_Reader_XYB::CT_Reader_XYB() : CT_AbstractReader()
{
    _filterRadius = 0;
}

QString CT_Reader_XYB::GetReaderName() const
{
    return tr("Points, Fichier XYB");
}

CT_StepsMenu::LevelPredefined CT_Reader_XYB::getReaderSubMenuName() const
{
    return CT_StepsMenu::LP_Points;
}

bool CT_Reader_XYB::setFilePath(const QString &filepath)
{
    // Test File validity
    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if(f.open(QIODevice::ReadOnly))
        {
            int      offset = 0;
            double   xc = 0;
            double   yc = 0;
            double   zc = 0;
            int      rows = 0;
            int      cols = 0;

            char d_data[8];
            int nb_0 = 0;

            while((offset<150) && (nb_0 != 4))
            {
                if (!f.atEnd())
                {
                    f.read(d_data, 1);

                    if(d_data[0] == 0)
                    {
                        ++nb_0;
                    }
                    else
                    {
                        nb_0 = 0;
                    }

                    ++offset;
                }
            }

            if(nb_0 != 4)
                offset = -1;

            f.close();

            // Read scan position
            bool okx = true;
            bool oky = true;
            bool okz = true;
            bool okr = true;
            bool okc = true;

            if (f.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream stream(&f);

                stream.readLine();
                QString line = stream.readLine();
                if (!line.isNull())
                {
                    QStringList values = line.split(" ");
                    if (values.size() >= 4 && values.at(0)=="ScanPosition")
                    {
                        xc = values.at(1).toDouble(&okx);
                        yc = values.at(2).toDouble(&oky);
                        zc = values.at(3).toDouble(&okz);
                    }
                }

                line = stream.readLine();
                if (!line.isNull())
                {
                    QStringList values = line.split(" ");
                    if (values.size() >= 2 && values.at(0)=="Rows")
                    {
                        rows = values.at(1).toDouble(&okr);
                    }
                }

                line = stream.readLine();
                if (!line.isNull())
                {
                    QStringList values = line.split(" ");
                    if (values.size() >= 2 && values.at(0)=="Cols")
                    {
                        cols = values.at(1).toDouble(&okc);
                    }
                }

                f.close();
            }

            if (okx && oky && okz && okr && okc && offset > 0) {
                if(CT_AbstractReader::setFilePath(filepath)) {
                    _xc = xc;
                    _yc = yc;
                    _zc = zc;
                    _rows = rows;
                    _cols = cols;
                    _offset = offset;

                    return true;
                }
            }
        }
    }

    return false;
}

void CT_Reader_XYB::setFilterRadius(const double &radius)
{
    _filterRadius = radius;
}

double CT_Reader_XYB::filterRadius() const
{
    return _filterRadius;
}

SettingsNodeGroup *CT_Reader_XYB::getAllSettings() const
{
    SettingsNodeGroup *root = new SettingsNodeGroup("Settings");
    root->addValue(new SettingsNodeValue("filterRadius", filterRadius()));

    return root;
}

bool CT_Reader_XYB::setAllSettings(const SettingsNodeGroup *settings)
{
    QList<SettingsNodeGroup*> listG = settings->groupsByTagName("Settings");

    if(!listG.isEmpty())
    {
        QList<SettingsNodeValue*> listV = listG.first()->valuesByTagName("filterRadius");

        if(listV.isEmpty())
            return false;

        setFilterRadius(listV.first()->value().toDouble());

        return true;
    }

    return false;
}

CT_AbstractReader* CT_Reader_XYB::copy() const
{
    return new CT_Reader_XYB();
}

void CT_Reader_XYB::protectedInit()
{
    addNewReadableFormat(FileFormat("xyb", tr("Fichiers binaire de points .xyb")));

    setToolTip(tr("Chargement de points depuis un fichier format binaire XYB (FARO)"));
}

void CT_Reader_XYB::protectedCreateOutItemDrawableModelList()
{
    CT_AbstractReader::protectedCreateOutItemDrawableModelList();

    addOutItemDrawableModel(DEF_CT_Reader_XYB_sceneOut, new CT_Scene(), tr("Scène"));
    addOutItemDrawableModel(DEF_CT_Reader_XYB_intensityOut, new CT_PointsAttributesScalarTemplated<quint16>(), tr("Intensité"));
    addOutItemDrawableModel(DEF_CT_Reader_XYB_scannerOut, new CT_Scanner(), tr("Scanner"));
}

bool CT_Reader_XYB::protectedReadFile()
{
    bool filter = (_filterRadius > 0);

    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if(f.open(QIODevice::ReadOnly))
        {
            QDataStream stream(&f);
            stream.setByteOrder(QDataStream::LittleEndian);
            stream.skipRawData(_offset);

            qint64 filesize = f.size();
            qint64 a = 0;

            // un tableau de n_points
            qint64 n_points = (filesize - _offset) / 26;

            CT_AbstractUndefinedSizePointCloud* mpcir;
            CT_NMPCIR pcir;
            CT_Point pReaded;
            CT_MutablePointIterator *it = NULL;

            if (filter)
                mpcir = PS_REPOSITORY->createNewUndefinedSizePointCloud();
            else {
                pcir = PS_REPOSITORY->createNewPointCloud(n_points);
                it = new CT_MutablePointIterator(pcir);
            }

            CT_StandardCloudStdVectorT<quint16> *collection;

            if(filter)
                collection = new CT_StandardCloudStdVectorT<quint16>();
            else
                collection = new CT_StandardCloudStdVectorT<quint16>(n_points);

            double xmin = std::numeric_limits<double>::max();
            double ymin = std::numeric_limits<double>::max();
            double zmin = std::numeric_limits<double>::max();
            quint16 imin = std::numeric_limits<quint16>::max();

            double xmax = -std::numeric_limits<double>::max();
            double ymax = -std::numeric_limits<double>::max();
            double zmax = -std::numeric_limits<double>::max();
            quint16 imax = 0;

            double x, y, z;
            quint16 reflectance;

            while(!stream.atEnd()
                    && !isStopped())
            {
                stream >> x;
                stream >> y;
                stream >> z;
                stream >> reflectance;

                if (filter)
                {
                    double dx = x - _xc;
                    double dy = y - _yc;
                    double distance2D = sqrt(dx*dx + dy*dy);

                    if (distance2D <= _filterRadius)
                    {
                        pReaded(CT_Point::X) = x;
                        pReaded(CT_Point::Y) = y;
                        pReaded(CT_Point::Z) = z;

                        if (x<xmin) {xmin = x;}
                        if (x>xmax) {xmax = x;}
                        if (y<ymin) {ymin = y;}
                        if (y>ymax) {ymax = y;}
                        if (z<zmin) {zmin = z;}
                        if (z>zmax) {zmax = z;}
                        if (reflectance<imin) {imin = reflectance;}
                        if (reflectance>imax) {imax = reflectance;}

                        mpcir->addPoint(pReaded);

                        collection->addT(reflectance);
                    }
                }
                else
                {
                    pReaded(CT_Point::X) = x;
                    pReaded(CT_Point::Y) = y;
                    pReaded(CT_Point::Z) = z;

                    if (x<xmin) {xmin = x;}
                    if (x>xmax) {xmax = x;}
                    if (y<ymin) {ymin = y;}
                    if (y>ymax) {ymax = y;}
                    if (z<zmin) {zmin = z;}
                    if (z>zmax) {zmax = z;}
                    if (reflectance<imin) {imin = reflectance;}
                    if (reflectance>imax) {imax = reflectance;}

                    it->next();
                    it->replaceCurrentPoint(pReaded);

                    (*collection)[a] = reflectance;
                }

                ++a;

                setProgress(a*100/n_points);
            }

            if (it != NULL) {delete it;}

            CT_Scene *scene;

            if(filter)
                pcir = PS_REPOSITORY->registerUndefinedSizePointCloud(mpcir);

            scene = new CT_Scene(NULL, NULL, pcir);
            scene->setBoundingBox(xmin, ymin, zmin, xmax, ymax, zmax);

            // add the scene
            addOutItemDrawable(DEF_CT_Reader_XYB_sceneOut, scene);

            CT_PointsAttributesScalarTemplated<quint16> *pas = new CT_PointsAttributesScalarTemplated<quint16>(NULL,
                                                                                                               NULL,
                                                                                                               pcir,
                                                                                                               collection,
                                                                                                               imin,
                                                                                                               imax);

            // add attributes
            addOutItemDrawable(DEF_CT_Reader_XYB_intensityOut, pas);

            if (_rows<=0 || _cols<=0)
                return true;

            double hres = 150.0 / ((double)_rows);
            double vres = 360.0 / ((double)_cols);

            // add the scanner
            addOutItemDrawable(DEF_CT_Reader_XYB_scannerOut, new CT_Scanner(NULL, NULL, 0, Eigen::Vector3d(_xc, _yc, _zc), Eigen::Vector3d(0,1,0), 360, 150, hres, vres, 0, 0, true, false));

            return true;
        }
    }

    return false;
}

