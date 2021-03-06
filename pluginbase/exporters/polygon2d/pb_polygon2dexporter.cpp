#include "pb_polygon2dexporter.h"
#include "ct_itemdrawable/ct_polygon2d.h"
#include <math.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include "ct_global/ct_context.h"

PB_Polygon2DExporter::PB_Polygon2DExporter() : CT_AbstractExporter()
{
    setCanExportItems(true);
    setCanExportPoints(false);
}

PB_Polygon2DExporter::~PB_Polygon2DExporter()
{

}

QString PB_Polygon2DExporter::getExporterCustomName() const
{
    return tr("Polygones 2D, ACSII");
}

CT_StepsMenu::LevelPredefined PB_Polygon2DExporter::getExporterSubMenuName() const
{
    return CT_StepsMenu::LP_Items;
}


void PB_Polygon2DExporter::init()
{
    addNewExportFormat(FileFormat("txt", tr("Polygones 2D (ASCII)")));

    setToolTip(tr("Exporte des Polygones 2D en format ASCII. Le fichier de sortie contient une ligne par polygones.<br>"
                  "Pour chaque polygone, sont exportés :<br>"
                  "- PolygonId : Identifiant Computree du polygone<br>"
                  "- AreaOfPolygon : Aire du polygone<br>"
                  "- Xcenter : Coordonnée X du centre de masse du polygone<br>"
                  "- Ycenter : Coordonnée Y du centre de masse du polygone<br>"
                  "- NumberOfVertice(N) : Nombre de sommets du polygone<br>"
                  "- Xvertice1 : Coordonnée X du 1er sommet<br>"
                  "- Yvertice1 : Coordonnée Y du 1er sommet<br>"
                  "- Xvertice2 : Coordonnée X du 2ième sommet<br>"
                  "- Yvertice2 : Coordonnée Y du 2ième sommet<br>"
                  "..."
                  "- XverticeN : Coordonnée X du Nième sommet<br>"
                  "- YverticeN : Coordonnée Y du Nième sommet<br>"
                  "N.B. : Le nombre de colonnes varie donc pour chaque ligne. "));}

bool PB_Polygon2DExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);
    int nProfiles = 0;

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();
        if(dynamic_cast<CT_Polygon2D*>(item) != NULL)
        {
            myList.append(item);
            ++nProfiles;
        }
    }

    if(nProfiles == 0)
    {
        setErrorMessage(tr("Aucun ItemDrawable du type CT_Polygon2D"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

bool PB_Polygon2DExporter::configureExport()
{
    if(!errorMessage().isEmpty())
    {
        QMessageBox::critical(NULL, tr("Erreur"), errorMessage(), QMessageBox::Ok);
        return false;
    }

    return true;
}

CT_AbstractExporter* PB_Polygon2DExporter::copy() const
{
    return new PB_Polygon2DExporter();
}

bool PB_Polygon2DExporter::protectedExportToFile()
{
    bool ok = true;

    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "txt";

    QString filePath = QString("%1/%2.%3").arg(path).arg(baseName).arg(suffix);
    QFile file(filePath);

    if(file.open(QFile::WriteOnly))
    {
        QTextStream stream(&file);

        // write header
        stream << "# 2D Polygons export from Computree v3\n";
        stream << "# Format:\n";
        stream << "PolygonId\tAreaOfPolygon\tXcenter\tYcenter\tNumberOfVertice(N)\tXvertice1\tYvertice1\tXvertice2\tYvertice2\t...\tXverticeN\tYverticeN\n";

        QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());
        while (it.hasNext())
        {
            CT_Polygon2D* item = dynamic_cast<CT_Polygon2D*>(it.next());
            item->computeCentroid();

            stream << item->id() << "\t";
            stream << item->getArea() << "\t";
            stream << item->getCenterX() << "\t";
            stream << item->getCenterY() << "\t";
            stream << item->getNumberOfVertices();

            if (item->getNumberOfVertices() > 0)
            {
                const QVector<Eigen::Vector2d*> &vertices = item->getVertices();
                for (int i = 0 ; i < vertices.size() ; i++)
                {
                    Eigen::Vector2d* vertice = vertices.at(i);

                    stream << "\t";
                    stream << (*vertice)(0) << "\t";
                    stream << (*vertice)(1);
                }
            }
            stream << "\n";
        }
        file.close();
    } else {ok = false;}

    return ok;
}
