#include "pb_stepexportpointsbyxyarea.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

#include "ct_abstractstepplugin.h"
#include "ct_exporter/ct_standardexporterseparator.h"
#include "ct_iterator/ct_pointiterator.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_loopcounter.h"

#include "exporters/profile/pb_profileexporter.h"
#include "ct_view/ct_stepconfigurabledialog.h"
#include "ct_exporter/abstract/ct_abstractexporterattributesselection.h"

#include <QDebug>

// Alias for indexing models
#define DEFin_resCount "resCount"
#define DEF_inGroupCount "grpCount"
#define DEF_inCounter "counter"

#define DEFin_resScene "resScene"
#define DEFin_grpScene "grpScene"
#define DEFin_itemScene "scene"


#define DEFin_resAreas "resArea"
#define DEFin_grpAreas "grpArea"
#define DEFin_itemAreas "area"

#define DEFout_res "res"
#define DEFout_grp "grp"


// Constructor : initialization of parameters
PB_StepExportPointsByXYArea::PB_StepExportPointsByXYArea(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _suffixFileName = "";
    _exporterConfiguration = NULL;


    // Create the available exporter map
    PluginManagerInterface *pm = PS_CONTEXT->pluginManager();
    int s = pm->countPluginLoaded();

    for(int i=0; i<s; ++i)
    {
        CT_AbstractStepPlugin *p = pm->getPlugin(i);

        QList<CT_StandardExporterSeparator*> rsl = p->getExportersAvailable();
        QListIterator<CT_StandardExporterSeparator*> itR(rsl);

        while(itR.hasNext())
        {
            CT_StandardExporterSeparator *rs = itR.next();
            QListIterator<CT_AbstractExporter*> itE(rs->exporters());

            while(itE.hasNext())
            {
                CT_AbstractExporter *exporter = itE.next();

                if (exporter != NULL && exporter->canExportPoints() && exporter->canExportPieceByPiece())
                {
                    CT_AbstractExporter *exporterCpy = exporter->copy();
                    exporterCpy->init();
                    _exportersInstancesList.append(exporterCpy);

                    const QList<FileFormat>& formats = exporterCpy->exportFormats();

                    for (int n = 0 ; n < formats.size() ; n++)
                    {
                        const FileFormat& format = formats.at(n);

                        QString key = QString("%2 - %1").arg(exporter->getExporterName()).arg(format.description());
                        _exportersMap.insert(key, exporterCpy);
                    }
                }
            }
        }
    }
}

PB_StepExportPointsByXYArea::~PB_StepExportPointsByXYArea()
{
    qDeleteAll(_exportersInstancesList);
    qDeleteAll(_areas);
    clearExporterConfiguration();
}


// Step description (tooltip of contextual menu)
QString PB_StepExportPointsByXYArea::getStepDescription() const
{
    return tr("Redallage de points dans une boucle");
}

// Step detailled description
QString PB_StepExportPointsByXYArea::getStepDetailledDescription() const
{
    return tr("Parmi les points de la scène d'entrée, séléctionne pour chaque emprise la liste des points à exporter."
              "Cet étape fonctionne dans une boucle, gardant le même fichier de sortie pour chaque emprise, pour toutes les scènes d'entrée (une par tour de boucle).");
}

// Step URL
QString PB_StepExportPointsByXYArea::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

bool PB_StepExportPointsByXYArea::setAllSettings(const SettingsNodeGroup *settings)
{
    if(!CT_AbstractStep::setAllSettings(settings))
        return false;

    QList<SettingsNodeGroup*> groups = settings->groupsByTagName("PB_StepExportPointsByXYArea");

    if(groups.isEmpty())
        return false;

    groups = groups.first()->groupsByTagName("ExporterConfiguration");

    if(groups.isEmpty())
        return false;

    if(groups.first()->groups().isEmpty())
        return !groups.first()->valuesByTagName("NOTHING").isEmpty();

    CT_AbstractExporter* exporter = (CT_AbstractExporter*) _exportersMap.value(_exportersListValue);
    exporter->setMyStep(this);

    CT_AbstractExporterAttributesSelection* exp = dynamic_cast<CT_AbstractExporterAttributesSelection*>(exporter);

    if (exp != NULL)
        exp->setSearchOnlyModels(true);

    SettingsNodeGroup *exporterConfiguration = groups.first()->groups().first();

    if(exporter->loadExportConfiguration(exporterConfiguration))
    {
        clearExporterConfiguration();
        _exporterConfiguration = exporter->saveExportConfiguration();

        return true;
    }

    return false;
}

SettingsNodeGroup *PB_StepExportPointsByXYArea::getAllSettings() const
{
    CT_AbstractExporter* exporter = (CT_AbstractExporter*) _exportersMap.value(_exportersListValue);

    SettingsNodeGroup *root = CT_AbstractStep::getAllSettings();
    SettingsNodeGroup *group = new SettingsNodeGroup("PB_StepExportPointsByXYArea");
    group->addValue(new SettingsNodeValue("Version", "1"));

    SettingsNodeGroup *confGroup = new SettingsNodeGroup("ExporterConfiguration");

    if(_exporterConfiguration != NULL)
    {
        // do a copy
        exporter->loadExportConfiguration(_exporterConfiguration);

        SettingsNodeGroup *exporterConfiguration = exporter->saveExportConfiguration();

        if(exporterConfiguration != NULL)
            confGroup->addGroup(exporterConfiguration);
        else
            confGroup->addValue(new SettingsNodeValue("NOTHING"));
    }

    group->addGroup(confGroup);
    root->addGroup(group);

    return root;
}

// Step copy method
CT_VirtualAbstractStep* PB_StepExportPointsByXYArea::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepExportPointsByXYArea(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepExportPointsByXYArea::createInResultModelListProtected()
{
    CT_InResultModelGroup* resCount = createNewInResultModel(DEFin_resCount, tr("Résultat compteur"), "", true);
    resCount->setRootGroup(DEF_inGroupCount);
    resCount->addItemModel(DEF_inGroupCount, DEF_inCounter, CT_LoopCounter::staticGetType(), tr("Compteur"));

    CT_InResultModelGroup *resScene = createNewInResultModel(DEFin_resScene, tr("Scène"));
    resScene->setZeroOrMoreRootGroup();
    resScene->addGroupModel("", DEFin_grpScene, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resScene->addItemModel(DEFin_grpScene, DEFin_itemScene, CT_AbstractItemDrawableWithPointCloud::staticGetType(), tr("Scène"));

    CT_InResultModelGroup *resXYAreas = createNewInResultModel(DEFin_resAreas, tr("Emprise"), "", true);
    resXYAreas->setZeroOrMoreRootGroup();
    resXYAreas->addGroupModel("", DEFin_grpAreas, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resXYAreas->addItemModel(DEFin_grpAreas, DEFin_itemAreas, CT_AbstractAreaShape2D::staticGetType(), tr("Emprise"));
}


// Creation and affiliation of OUT models
void PB_StepExportPointsByXYArea::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res = createNewOutResultModel(DEFout_res, tr("Résultat"));
    res->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepExportPointsByXYArea::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();
    configDialog->addFileChoice(tr("Répertoire d'export"), CT_FileChoiceButton::OneExistingFolder, "", _dir);
    configDialog->addString(tr("Suffixe de nom de fichier"), "", _suffixFileName);

    QStringList list_exportersList;

    QMapIterator<QString, CT_AbstractExporter*> it(_exportersMap);
    while (it.hasNext())
    {
        it.next();
        list_exportersList.append(it.key());
    }

    if (list_exportersList.isEmpty())
    {
        list_exportersList.append(tr("ERREUR : aucun exporter disponible"));
    }

    configDialog->addStringChoice(tr("Choix du type de fichier"), "", list_exportersList, _exportersListValue);

}

bool PB_StepExportPointsByXYArea::postConfigure()
{
    if(CT_VirtualAbstractStep::postConfigure())
        return configureExporter();


    return true;
}

bool PB_StepExportPointsByXYArea::configureExporter()
{
    CT_AbstractExporter* exporter = (CT_AbstractExporter*) _exportersMap.value(_exportersListValue);
    exporter->setMyStep(this);

    CT_AbstractExporterAttributesSelection* exp = dynamic_cast<CT_AbstractExporterAttributesSelection*>(exporter);
    if (exp != NULL)
    {
        exp->setSearchOnlyModels(true);
    }

    if(_exporterConfiguration != NULL)
        exporter->loadExportConfiguration(_exporterConfiguration);

    if(exporter->configureExport())
    {
        clearExporterConfiguration();
        _exporterConfiguration = exporter->saveExportConfiguration();

        setSettingsModified(true);

        return true;
    }

    return false;
}

void PB_StepExportPointsByXYArea::clearExporterConfiguration()
{
    delete _exporterConfiguration;
    _exporterConfiguration = NULL;
}

void PB_StepExportPointsByXYArea::compute()
{

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resInCounter = inResultList.at(0);
    CT_ResultGroup* resInScene = inResultList.at(1);
    CT_ResultGroup* resInAreas = inResultList.at(2);

    setProgress(5);

    // First turn: create export files
    CT_LoopCounter* counter = NULL;

    CT_ResultItemIterator it_Counter(resInCounter, this, DEF_inCounter);
    if (it_Counter.hasNext())
    {
        counter = (CT_LoopCounter*) it_Counter.next();

        if (counter != NULL && counter->getCurrentTurn() == 1)
        {
            CT_AbstractExporter* exporter = (CT_AbstractExporter*) _exportersMap.value(_exportersListValue);

            if (exporter != NULL)
            {
                CT_ResultGroupIterator itIn_grpAreas(resInAreas, this, DEFin_grpAreas);
                while (itIn_grpAreas.hasNext() && !isStopped())
                {
                    const CT_AbstractItemGroup* group = (CT_AbstractItemGroup*) itIn_grpAreas.next();

                    CT_AbstractAreaShape2D* area = (CT_AbstractAreaShape2D*)group->firstItemByINModelName(this, DEFin_itemAreas);

                    if (area != NULL)
                    {
                        CT_AbstractExporter* exporterCpy = exporter->copy();
                        exporterCpy->init();

                        exporterCpy->setMyStep(this);

                        CT_AbstractExporterAttributesSelection* exp = dynamic_cast<CT_AbstractExporterAttributesSelection*>(exporterCpy);
                        if (exp != NULL)
                        {
                            exp->setSearchOnlyModels(true);
                        }


                        if (exporterCpy != NULL)
                        {
                            QString path = "";
                            if (_dir.size() > 0)
                            {
                                path.append(_dir.first());
                                path.append("/");
                            }

                            path.append(area->displayableName());
                            path.append(_suffixFileName);

                            if (exporterCpy->setExportFilePath(path) && exporterCpy->createExportFileForPieceByPieceExport())
                            {
                                CT_AreaShape2DData* areaBox2D = (CT_AreaShape2DData*) (area->getPointerData()->copy());
                                AreaData* areaData = new AreaData(areaBox2D, exporterCpy);
                                _areas.append(areaData);
                            }
                        }
                    }
                }
            }
        }
    }

    for (int i = 0 ; i < _areas.size() ; i++)
    {
        AreaData* areaData = _areas.at(i);
        areaData->setPointCloudIndex(new CT_PointCloudIndexVector());
    }

    setProgress(10);

    // Point export
    CT_ResultGroupIterator itIn_grpScene(resInScene, this, DEFin_grpScene);
    while (itIn_grpScene.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup* group = (CT_AbstractItemGroup*) itIn_grpScene.next();
        
        CT_AbstractItemDrawableWithPointCloud* scene = (CT_AbstractItemDrawableWithPointCloud*)group->firstItemByINModelName(this, DEFin_itemScene);

        if (scene != NULL)
        {
            const CT_AbstractPointCloudIndex* inCloudIndex = scene->getPointCloudIndex();

            CT_PointIterator itP(inCloudIndex);
            while(itP.hasNext() && (!isStopped()))
            {
                const CT_Point &point = itP.next().currentPoint();
                size_t index = itP.currentGlobalIndex();

                for (int i = 0 ; i < _areas.size() ; i++)
                {
                    AreaData* areaData = _areas.at(i);
                    CT_AreaShape2DData* area = areaData->_area;

                    if (area->contains(point(0), point(1)))
                    {
                        CT_PointCloudIndexVector* outCloudIndex = areaData->_cloudIndex;
                        outCloudIndex->addIndex(index);
                    }
                }
            }
        }
    }


    setProgress(80);


    for (int i = 0 ; i < _areas.size() ; i++)
    {
        AreaData* areaData = _areas.at(i);

        CT_PointCloudIndexVector* outCloudIndex = areaData->_cloudIndex;
        CT_AbstractExporter* exporter = areaData->_exporter;

        QList<CT_AbstractCloudIndex *> list;
        list.append(outCloudIndex);

        exporter->loadExportConfiguration(_exporterConfiguration);
        exporter->setPointsToExport(list);
        exporter->exportOnePieceOfDataToFile();

        areaData->deletePointCloudIndex();
    }

    setProgress(90);

    // Last turn : finalize export files
    if (counter != NULL && counter->getCurrentTurn() == counter->getNTurns())
    {
        for (int i = 0 ; i < _areas.size() ; i++)
        {
            CT_AbstractExporter* exporter = _areas.at(i)->_exporter;
            exporter->finalizePieceByPieceExport();
        }
    }

    // OUT results creation
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* resOut = outResultList.at(0);
    CT_StandardItemGroup* grpOut= new CT_StandardItemGroup(DEFout_grp, resOut);
    resOut->addGroup(grpOut);

}
