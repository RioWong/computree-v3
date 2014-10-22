#include "pb_xybexporter.h"

#include <math.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QEventLoop>
#include <QApplication>
#include <QProgressDialog>
#include <QFileInfo>

#include "ct_global/ct_context.h"
#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"
#include "ct_itemdrawable/ct_scanner.h"
#include "ct_tools/itemdrawable/ct_itemdrawablecollectionbuildert.h"
#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"

PB_XYBExporter::PB_XYBExporter() : CT_AbstractExporterPointAttributesSelection()
{
    setCanExportItems(true);
    setCanExportPoints(true);
    m_scanner = NULL;
}

PB_XYBExporter::~PB_XYBExporter()
{
}

QString PB_XYBExporter::getExporterCustomName() const
{
    return "Points (X,Y,Z,I), format xyb";
}

void PB_XYBExporter::init()
{
    addNewExportFormat(FileFormat("xyb", tr("Fichiers binaire de points .xyb")));
}

bool PB_XYBExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable *> &list)
{
    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if(dynamic_cast<CT_IAccessPointCloud*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(errorMessage() + "\r\n" + tr("- Aucun ItemDrawable du type CT_IAccessPointCloud"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

bool PB_XYBExporter::setPointsToExport(const QList<CT_AbstractCloudIndex*> &list)
{
    QList<CT_AbstractCloudIndex*> myList;
    QListIterator<CT_AbstractCloudIndex*> it(list);

    while(it.hasNext())
    {
        CT_AbstractCloudIndex *item = it.next();

        if(dynamic_cast<CT_AbstractCloudIndexT<CT_Point>*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(errorMessage() + "\r\n" + tr("- Aucun points à exporter"));
        return false;
    }

    return CT_AbstractExporter::setPointsToExport(myList);
}

SettingsNodeGroup* PB_XYBExporter::saveExportConfiguration() const
{
    SettingsNodeGroup *root = CT_AbstractExporter::saveExportConfiguration();

    /*SettingsNodeGroup *myRoot = new SettingsNodeGroup("PB_XYBExporter");

    if(!m_attributes.isEmpty())
    {
        QListIterator< PBACS > it(m_attributes);

        while(it.hasNext())
        {
            PBACS pas = it.next();
            QListIterator<PBACR> itR(pas.results);

            while(itR.hasNext())
            {
                PBACR par = itR.next();
                QListIterator<IPointAttributes*> itA(par.collection);

                while(itA.hasNext())
                {
                    IPointAttributes *pa = itA.next();

                    SettingsNodeGroup *paG = new SettingsNodeGroup("PointAttributes");

                    SettingsNodeValue *paV = new SettingsNodeValue("step", pas.step->getStepName());
                    paG->addValue(paV);

                    paV = new SettingsNodeValue("result", par.result->getModel()->name());
                    paG->addValue(paV);

                    paV = new SettingsNodeValue("attributes", pa->getModel()->name());
                    paG->addValue(paV);

                    paV = new SettingsNodeValue("checked", (m_worker.m_attributes.contains(pa) ? "true" : "false"));
                    paG->addValue(paV);

                    myRoot->addGroup(paG);
                }
            }
        }
    }

    root->addGroup(myRoot);*/

    return root;
}

bool PB_XYBExporter::loadExportConfiguration(const SettingsNodeGroup *root)
{
    /*m_worker.m_attributes.clear();

    QList<SettingsNodeGroup*> groups = root->groupsByTagName("PB_XYBExporter");

    if(groups.isEmpty())
        return false;

    groups = groups.first()->groupsByTagName("PointAttributes");

    QListIterator< SettingsNodeGroup* > itGroups(groups);
    QListIterator< PBACS > it(m_attributes);

    while(it.hasNext())
    {
        PBACS pas = it.next();
        QListIterator<PBACR> itR(pas.results);

        while(itR.hasNext())
        {
            PBACR par = itR.next();
            QListIterator<IPointAttributes*> itA(par.collection);

            while(itA.hasNext())
            {
                if(!itGroups.hasNext())
                    return false;

                SettingsNodeGroup *group = itGroups.next();
                IPointAttributes *pa = itA.next();

                QList<SettingsNodeValue*> values = group->valuesByTagName("result");

                if(values.isEmpty() || (values.first()->value().toString() != par.result->getModel()->name()))
                    return false;

                values = group->valuesByTagName("attributes");

                if(values.isEmpty() || (values.first()->value().toString() != pa->getModel()->name()))
                    return false;

                values = group->valuesByTagName("checked");

                if(values.isEmpty())
                    return false;

                if(values.first()->value().toString() == "true")
                    m_worker.m_attributes.append(pa);
            }
        }
    }*/

    return CT_AbstractExporter::loadExportConfiguration(root);
}

QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > PB_XYBExporter::getBuilders() const
{
    QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > l = CT_AbstractExporterPointAttributesSelection::getBuilders();

    m_scannerBuilderIndex = l.size();
    l << QPair<QString, CT_AbstractItemDrawableCollectionBuilder*>(tr("Scanners"), (CT_AbstractItemDrawableCollectionBuilder*)&m_scannerBuilder);

    return l;
}

void PB_XYBExporter::setExcludeConfiguration(const QPair<QString, CT_AbstractItemDrawableCollectionBuilder *> &pair, CT_ItemDrawableHierarchyCollectionSelectionModel *model) const
{
    CT_AbstractExporterPointAttributesSelection::setExcludeConfiguration(pair, model);

    // scanners
    if((&m_scannerBuilder) == pair.second)
    {
        model->addExcludeModel(m_scannerBuilderIndex); // exclude me
    }
}

bool PB_XYBExporter::useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget)
{
    if(CT_AbstractExporterPointAttributesSelection::useSelection(selectorWidget))
    {
        QList<CT_AbstractSingularItemDrawable*> list = selectorWidget->itemDrawableSelected();
        QListIterator<CT_AbstractSingularItemDrawable*> it(list);

        m_scanner = NULL;

        while(it.hasNext() && (m_scanner == NULL))
            m_scanner = dynamic_cast<CT_Scanner*>(it.next());

        return true;
    }

    return false;
}

CT_AbstractExporter* PB_XYBExporter::copy() const
{
    return new PB_XYBExporter();
}

bool PB_XYBExporter::protectedExportToFile()
{
    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "xyb";
    QString filePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

    QFile file(filePath);

    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream txtStream(&file);

        txtStream << "# SCENE XYZ binary format v1.0\n";

        if(m_scanner != NULL)
        {
            CT_AbstractCoordinateSystem::realIm x, y, z;
            CT_AbstractCoordinateSystem::realEx xc, yc, zc;
            x = m_scanner->getPosition().x();
            y = m_scanner->getPosition().y();
            z = m_scanner->getPosition().z();

            PS_COORDINATES_SYS->convertExport(x, y, z, xc, yc, zc);

            txtStream << "ScanPosition " << (double)xc << " " << (double)yc << " " << (double)zc << "\n";
            txtStream << "Rows " << (m_scanner->getVFov()*RAD2DEG)/(m_scanner->getHRes()*RAD2DEG) << "\n";
            txtStream << "Cols " << (m_scanner->getHFov()*RAD2DEG)/(m_scanner->getVRes()*RAD2DEG) << "\n";
        }
        else
        {
            txtStream << "ScanPosition 0.00000000 0.00000000 0.00000000 \n";
            txtStream << "Rows 0\n";
            txtStream << "Cols 0\n";
        }

        file.close();
    }

    if(file.open(QFile::Append))
    {
        CT_AbstractColorCloud *cc = createColorCloudBeforeExportToFile();

        QDataStream stream(&file);
        stream.setByteOrder(QDataStream::LittleEndian);

        char d_data[8];

        // write header
        d_data[0] = 0;
        d_data[1] = 0;
        d_data[2] = 0;
        d_data[3] = 0;

        stream.writeRawData(d_data, 4);

        int totalToExport = itemDrawableToExport().size();

        const QList<CT_AbstractCloudIndex*> &pointsSelected = pointsToExport();

        int nExported = 0;

        // write data
        QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

        while(it.hasNext())
        {
            CT_AbstractItemDrawable *item = it.next();

            exportPoints(stream,
                         dynamic_cast<CT_IAccessPointCloud*>(item)->getPointCloudIndex(),
                         cc,
                         nExported,
                         totalToExport);

            nExported += 100;
        }

        QListIterator<CT_AbstractCloudIndex*> itCI(pointsSelected);
        totalToExport = pointsSelected.size();

        while(itCI.hasNext())
        {
            exportPoints(stream,
                         dynamic_cast<CT_AbstractCloudIndexT<CT_Point>*>(itCI.next()),
                         cc,
                         nExported,
                         totalToExport);

            nExported += 100;
        }

        clearWorker();

        file.close();
        return true;
    }

    clearWorker();

    return false;
}

void PB_XYBExporter::clearWorker()
{
    CT_AbstractExporterPointAttributesSelection::clearWorker();

    m_scanner = NULL;
}

void PB_XYBExporter::exportPoints(QDataStream &stream,
                                  const CT_AbstractCloudIndexT<CT_Point> *constPCIndex,
                                  const CT_AbstractColorCloud *cc,
                                  const int &nExported,
                                  const int &totalToExport)
{
    size_t totalSize = constPCIndex->size();
    size_t i = 0;

    CT_AbstractCloudIndexT<CT_Point>::ConstIterator begin = constPCIndex->constBegin();
    CT_AbstractCloudIndexT<CT_Point>::ConstIterator end = constPCIndex->constEnd();

    CT_AbstractCoordinateSystem::realEx xc, yc, zc;

    while(begin != end)
    {
        const CT_Point &point = begin.cT();

        PS_COORDINATES_SYS->convertExport(point.getX(), point.getY(), point.getZ(), xc, yc, zc);

        stream << ((double)xc);
        stream << ((double)yc);
        stream << ((double)zc);

        if(cc == NULL)
        {
            stream << (quint16)0;
        }
        else
        {
            const CT_Color &col = cc->constColorAt(begin.cIndex());
            quint16 tmp = (quint16)col.b;
            stream << tmp;
        }

        ++begin;
        ++i;

        setExportProgress((((i*100)/totalSize)+nExported)/totalToExport);
    }
}

