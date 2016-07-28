#include "pb_meshobjexporter.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "ct_itemdrawable/ct_meshmodel.h"
#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_iterator/ct_faceiterator.h"

PB_MeshObjExporter::PB_MeshObjExporter() : CT_AbstractExporter()
{
}

QString PB_MeshObjExporter::getExporterCustomName() const
{
    return tr("Meshs, format OBJ");
}

CT_StepsMenu::LevelPredefined PB_MeshObjExporter::getExporterSubMenuName() const
{
    return CT_StepsMenu::LP_Meshes;
}

void PB_MeshObjExporter::init()
{
    addNewExportFormat(FileFormat("obj", tr("Fichiers .obj")));

    setToolTip(tr("Exporte un maillage dans un fichier au format OBJ (Objet 3D)"));
}


bool PB_MeshObjExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable *> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if(dynamic_cast<CT_MeshModel*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(tr("Aucun ItemDrawable du type CT_MeshModel"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

CT_AbstractExporter* PB_MeshObjExporter::copy() const
{
    return new PB_MeshObjExporter();
}

bool PB_MeshObjExporter::protectedExportToFile()
{
    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "obj";
    QString filePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

    QFile file(filePath);

    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream stream(&file);
        stream << "# " << QFileInfo(filePath).fileName() << endl << endl;

        int totalToExport = itemDrawableToExport().size();
        int nExported = 0;

        // write data
        QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

        int lastPointCount = 0;
        int ptNb = 0;
        while(it.hasNext())
        {
            CT_AbstractItemDrawable *item = it.next();

            stream << "o " << item->id() << endl << endl;

            CT_Mesh *mesh = dynamic_cast<CT_MeshModel*>(item)->mesh();

            if(mesh != NULL)
            {
                CT_PointIterator itP(mesh->abstractVert());
                CT_FaceIterator itF(mesh->abstractFace());

                QHash<size_t, size_t> hashTablePoint;

                size_t totalSize = itP.size();
                size_t i = 0;

                while(itP.hasNext())
                {
                    const CT_Point &point = itP.next().currentPoint();
                    hashTablePoint.insert(itP.cIndex(), i+1);

                    stream << "v " << ((double)point(0)) << " " << ((double)point(1)) << " " << ((double)point(2)) << endl;

                    ++i;++ptNb;

                    setExportProgress((((i*50)/totalSize)+nExported)/totalToExport);
                }

                stream << endl;

                totalSize = itF.size();
                i = 0;

                while(itF.hasNext())
                {
                    const CT_Face &face = itF.next().cT();

                    stream << "f " << (hashTablePoint.value(face.iPointAt(0)) + lastPointCount) << " " << (hashTablePoint.value(face.iPointAt(1)) + lastPointCount) << " " << (hashTablePoint.value(face.iPointAt(2)) + lastPointCount) << endl;

                    ++i;

                    setExportProgress(((50+((i*50)/totalSize))+nExported)/totalToExport);
                }
            }

            nExported += 100;
            stream << endl;
            lastPointCount = ptNb;

        }

        file.close();
        return true;
    }

    return false;
}
