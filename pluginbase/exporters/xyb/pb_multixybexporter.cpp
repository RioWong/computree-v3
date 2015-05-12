#include "pb_multixybexporter.h"

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
#include "ct_itemdrawable/ct_scene.h"
#include "ct_tools/itemdrawable/ct_itemdrawablecollectionbuildert.h"
#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_point.h"

PB_MultiXYBExporter::PB_MultiXYBExporter() : CT_AbstractExporterPointAttributesSelection()
{
    setCanExportItems(true);
    setCanExportPoints(false);
    setCanExportWithColors(true);
    setCanExportWithNormals(false);
}

PB_MultiXYBExporter::~PB_MultiXYBExporter()
{
}

QString PB_MultiXYBExporter::getExporterCustomName() const
{
    return "Points (1 file/scene), XYB (X,Y,Z,I)";
}

void PB_MultiXYBExporter::init()
{
    addNewExportFormat(FileFormat("xyb", tr("Fichiers .xyb (1 fichier/scène)")));
}

bool PB_MultiXYBExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable *> &list)
{
    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if(dynamic_cast<CT_Scene*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(errorMessage() + "\r\n" + tr("- Aucun ItemDrawable du type CT_Scene"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}


SettingsNodeGroup* PB_MultiXYBExporter::saveExportConfiguration() const
{
    SettingsNodeGroup *root = CT_AbstractExporter::saveExportConfiguration();
    return root;
}

bool PB_MultiXYBExporter::loadExportConfiguration(const SettingsNodeGroup *root)
{
    return CT_AbstractExporter::loadExportConfiguration(root);
}

QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > PB_MultiXYBExporter::getBuilders() const
{
    return CT_AbstractExporterPointAttributesSelection::getBuilders();
}


void PB_MultiXYBExporter::setExcludeConfiguration(const QPair<QString, CT_AbstractItemDrawableCollectionBuilder *> &pair, CT_ItemDrawableHierarchyCollectionSelectionModel *model) const
{
    CT_AbstractExporterPointAttributesSelection::setExcludeConfiguration(pair, model);
}

bool PB_MultiXYBExporter::useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget)
{
    if(CT_AbstractExporterPointAttributesSelection::useSelection(selectorWidget))
    {
        return true;
    }
    return false;
}


CT_AbstractExporter* PB_MultiXYBExporter::copy() const
{
    return new PB_MultiXYBExporter();
}

bool PB_MultiXYBExporter::protectedExportToFile()
{
    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "xyb";

    // Loop on the items
    int cpt = 0;
    int size = itemDrawableToExport().size();
    QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());
    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        QString filePath = QString("%1/%2_%4.%3").arg(path).arg(baseName).arg(suffix).arg(item->id());
        QFile file(filePath);

        if(file.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream txtStream(&file);
            txtStream << "# SCENE XYZ binary format v1.0\n";
            txtStream << "ScanPosition 0.00000000 0.00000000 0.00000000 \n";
            txtStream << "Rows 0\n";
            txtStream << "Cols 0\n";
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

            // write data
            exportPoints(stream, dynamic_cast<CT_Scene*>(item)->getPointCloudIndex(), cc);

            file.close();

            PS_LOG->addMessage(LogInterface::info, LogInterface::exporter, tr("Fichier %1 créé").arg(filePath));

        } else {
            PS_LOG->addMessage(LogInterface::error, LogInterface::exporter, tr("Errreur : impossible de créer le fichier %1").arg(filePath));
        }

        setExportProgress(100.0*(float)cpt/(float)size);
    }

    clearWorker();

    return true;
}

void PB_MultiXYBExporter::clearWorker()
{
    CT_AbstractExporterPointAttributesSelection::clearWorker();
}

void PB_MultiXYBExporter::exportPoints(QDataStream &stream,
                                  const CT_AbstractPointCloudIndex *constPCIndex,
                                  const CT_AbstractColorCloud *cc)
{
    CT_PointIterator it(constPCIndex);

    while(it.hasNext())
    {
        const CT_Point &point = it.next().currentPoint();

        stream << point(CT_Point::X);
        stream << point(CT_Point::Y);
        stream << point(CT_Point::Z);

        if(cc == NULL)
        {
            stream << (quint16)0;
        }
        else
        {
            const CT_Color &col = cc->constColorAt(it.cIndex());
            quint16 tmp = (quint16)col.b;
            stream << tmp;
        }
    }
}

