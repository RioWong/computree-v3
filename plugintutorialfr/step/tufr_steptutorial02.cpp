// Inclusion du fichier d'ent�te
#include "tufr_steptutorial02.h"

// Inclusion des mod�les de r�sultats in et out
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

// Inclusion des mod�les de groupes in et out
#include "ct_itemdrawable/model/inModel/ct_inoneormoregroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instandardgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h"

// Inclusion des mod�les d'items in et out
#include "ct_itemdrawable/model/inModel/ct_instandarditemdrawablemodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandarditemdrawablemodel.h"

// Inclusion de la classe de r�sultat standard
#include "ct_result/ct_resultgroup.h"

// Inclusion des CT_ItemDrawables utiles dans l'�tape
#include "ct_itemdrawable/ct_scene.h"

// Inclusion des autres �l�ments n�cessaires � la cr�ation d'une sc�ne
#include "ct_pointcloudindexvector.h"
#include "ct_element/ct_axisalignedboundingbox.h"

#include "qdebug.h" // pour afficher des messages � la console

#include "math.h"

// Alias des chaines de caract�res pour l'indexation des mod�les in et out
#define DEF_SearchInResult  "r"
#define DEF_SearchInScene   "sc"

#define DEF_SearchOutResult "r"
#define DEF_SearchOutGroup  "g"
#define DEF_SearchOutScene  "sc"

// Constructeur : appel du constructeur de la classe m�re
//                Initialisation des param�tres (valeurs par d�faut)
TUFR_StepTutorial02::TUFR_StepTutorial02(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _zmin = 0;
    _zmax = 1;
}

// Description de l'�tape (tooltip du menu contextuel)
QString TUFR_StepTutorial02::getStepDescription() const
{
    return "Extrait une placette entre zmin et zmax";
}

// M�thode de recopie de l'�tape
CT_VirtualAbstractStep* TUFR_StepTutorial02::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette �tape
    return new TUFR_StepTutorial02(dataInit);
}

//////////////////// PROTECTED //////////////////

// Cr�ation et affiliation des mod�les IN
void TUFR_StepTutorial02::createInResultModelListProtected()
{
    // D�claration et cr�ation du mod�le de groupe racine
    // Ici on utilise un CT_InOneOrMoreGroupModel, pour permettre
    // d'utiliser une sc�ne n'importe o� dans une hi�rachie des groupes
    CT_InOneOrMoreGroupModel *groupModel;
    groupModel = new CT_InOneOrMoreGroupModel();

    // D�claration et cr�ation du mod�le d'item sc�ne
    // Le 1er param�tre sp�cifie la classe souhait�e pour l'Item NomDeClasse::staticGetType()
    // Le 2ieme param�tre indique si plusieurs candidats peuvent �tre s�l�ctionn�s dans le r�sultat d'entr�e
    // Ici on oblige � n'en choisir qu'un seul
    // Le 3ieme param�tre est l'alias d'indexation de ce mod�le
    // Le 4ieme param�tre indique si cet item est obligatoire (c'est le cas ici) ou facultatif
    // N.B. : il faut au moins un item obligatoire dans le cas g�n�ral
    // Le 5ieme param�tre donne un nom � ce mod�le d'item affichable par l'interface
    // Le 6ieme param�tre donne une description � ce mod�le d'item affichable par l'interface
    CT_InStandardItemDrawableModel *sceneItemModel;
    sceneItemModel = new CT_InStandardItemDrawableModel(CT_Scene::staticGetType(),
                             CT_InStandardItemDrawableModel::C_ChooseOneIfMultiple,
                             DEF_SearchInScene,
                             CT_InStandardItemDrawableModel::F_IsObligatory,
                             "Sc�ne",
                             "Objet CT_Scene");

    // On ajoute le mod�le d'item (sc�ne) au mod�le du groupe racine
    groupModel->addItem(sceneItemModel);

    // D�claration et cr�ation du mod�le de r�sultat
    CT_InResultModelGroup *resultModel;
    // Le 1er param�tre est le mod�le du groupe racine
    // Le 2ieme param�tre est l'alias d'indexation du mod�le de r�sultat
    // Le 3ieme param�tre indique si on doit chercher le r�sultat d'entr�e
    // r�curssivement en remontant l'arbre des �tapes (ici non : false)
    // Le 4ieme param�tre donne une description � ce mod�le de r�sultat, affichable par l'interface
    // Le 5ieme param�tre donne un nom � ce mod�le de r�sultat, affichable par l'interface
    resultModel = new CT_InResultModelGroup(groupModel,
                                            DEF_SearchInResult,
                                            false,
                                            "Un r�sultat avec une/des sc�nes",
                                            "R�sultat avec une Sc�ne");

    // Ajout du mod�le de r�sultat s�quenciellement dans cette �tape
    // En r�alit� cette m�thode g�n�re aussi un resulat auquel est attach� ce mod�le
    // Les addInResultModel susccessifs, ajoutent des r�sultats dans l'ordre � la InResultList
    addInResultModel(resultModel);
}

// Cr�ation et affiliation des mod�les OUT
void TUFR_StepTutorial02::createOutResultModelListProtected()
{
    // D�claration et cr�ation du mod�le de groupe racine
    CT_OutStandardGroupModel *groupModel;
    groupModel = new CT_OutStandardGroupModel(DEF_SearchOutGroup);

    // D�claration et cr�ation du mod�le d'item sc�ne
    // Le 1er param�tre sp�cifie la classe qui sera cr�e, en fournissant une instance de cette classe
    // Le 2ieme param�tre est l'alias d'indexation de ce mod�le
    // Le 3ieme param�tre donne un nom � ce mod�le d'item affichable par l'interface
    // Le 4ieme param�tre donne une description � ce mod�le d'item affichable par l'interface
    CT_OutStandardItemDrawableModel *sceneItemModel;
    sceneItemModel = new CT_OutStandardItemDrawableModel(new CT_Scene(),
                             DEF_SearchOutScene,
                             "Sc�ne extraite",
                             "Sc�ne extraite");

    // On ajoute le mod�le d'item (sc�ne) au mod�le du groupe racine
    groupModel->addItem(sceneItemModel);

    // D�claration et cr�ation du mod�le de r�sultat
    CT_OutResultModelGroup *resultModel;
    // Le 1er param�tre est le mod�le du groupe racine
    // Le 2ieme param�tre est l'alias d'indexation du mod�le de r�sultat
    // Le 3ieme param�tre donne un nom au r�sultat
    // Le 4ieme param�tre donne un nom � ce mod�le de r�sultat affichable par l'interface
    // Le 5ieme param�tre donne une decription � ce mod�le de r�sultat affichable par l'interface
    resultModel = new CT_OutResultModelGroup(groupModel,
                                             DEF_SearchOutResult,
                                             "ExtractedPlot",
                                             "Sc�ne extraite",
                                             "Sc�ne extraite");

    // Ajout du mod�le de r�sultat s�quenciellement dans cette �tape
    // En r�alit� cette m�thode g�n�re aussi un resulat auquel est attach� ce mod�le
    // Les addOutResultModel susccessifs, ajoutent des r�sultats dans l'ordre � la OutResultList
    addOutResultModel(resultModel);
}

// Cr�ation semi-automatique de la boite de dialogue de param�trage de l'�tape
void TUFR_StepTutorial02::createPostConfigurationDialog()
{
    // Cr�ation de la boite de dialog en elle-m�me
    // La m�thode newStandardPostConfigurationDialog cr�e la boite et l'affecte � l'�tape en cours
    // Ainsi la gestion et la suppression de cette boite sont g�r�s automatiquement
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    // Ajout de contr�les unitaires de param�trages � la boite (un par param�tre en g�n�ral)
    configDialog->addDouble("Z minimum :", "m", -10000, 10000, 2, _zmin);
    configDialog->addDouble("Z maximum :", "m", -10000, 10000, 2, _zmax);
}

void TUFR_StepTutorial02::compute()
{
    // RESULTATS IN
    // R�cup�ration de la liste des r�sultats d'entr�e s�l�ctionn� par createInResultModelListProtected()
    // L'ordre dans cette liste est celui des addInResultModel() successifs
    QList<CT_ResultGroup*> inResultList = getInputResults();

    // Ici on a fait un seul addInResultModel(), donc on a un seul r�sultat � r�cup�rer : le premier
    CT_ResultGroup* inResult;
    inResult = inResultList.at(0);

    // On va r�cup�rer le mod�le OUT de ce r�sultat d'entr�e
    CT_OutAbstractResultModel* inResultOutModel = inResult->model();

    // On va r�cup�rer les mod�les IN des groupes et items que l'on souhaite indexer
    CT_InStandardItemDrawableModel *inSceneModel = (CT_InStandardItemDrawableModel*) getInModel(*inResultOutModel, DEF_SearchInScene);


    // RESULTATS OUT
    // R�cup�ration de la liste des r�sultats de sortie cr��s par createOutResultModelListProtected()
    // L'ordre dans cette liste est celui des addOutResultModel() successifs
    QList<CT_ResultGroup*> outResultList = getOutResultList();

    // Ici on a fait un seul addOutResultModel(), donc on a un seul r�sultat � r�cup�rer : le premier
    CT_ResultGroup* outResult;
    outResult = outResultList.at(0);

    // On va r�cup�rer le mod�le OUT de ce r�sultat de sortie
    CT_OutAbstractResultModel* outResultOutModel = outResult->model();

    // On va r�cup�rer les mod�les OUT des groupes et items que l'on souhaite cr�er
    CT_OutStandardGroupModel* groupModel = (CT_OutStandardGroupModel*) getOutModel(*outResultOutModel, DEF_SearchOutGroup);
    CT_OutStandardItemDrawableModel* sceneItemModel = (CT_OutStandardItemDrawableModel*) getOutModel(*outResultOutModel, DEF_SearchOutScene);


    // ALGORITHME
    // Cr�ation d'un it�rateur sur les items ayant pour mod�le : inSceneModel
    if (inResult->recursiveBeginIterateItems(*inSceneModel))
    {
        // r�cup�ration du premier groupe contenant un item de ce type
        CT_AbstractItemGroup *itemGroup = inResult->recursiveNextItem();

        // on s'assure qu'il exite
        if (itemGroup!=NULL)
        {
            // On r�cup�re la premi�re (la seule ici) sc�ne correspondant au mod�le dans ce groupe
            const CT_Scene *scene = (CT_Scene*) itemGroup->findFirstItem(*inSceneModel);

            // On s'assure qu'elle existe
            if (scene != NULL)
            {
                const CT_AbstractPointCloud *pointcloud = scene->getPointCloud();
                const CT_AbstractPointCloudIndex *pointcloudIndex = scene->getPointCloudIndex();

                CT_PointCloudIndexVector *newIndex = new CT_PointCloudIndexVector();

                double xmin = LONG_MAX;
                double xmax = LONG_MIN;
                double ymin = LONG_MAX;
                double ymax = LONG_MIN;
                double zmin = LONG_MAX;
                double zmax = LONG_MIN;

                int cloudSize = pointcloudIndex->indexSize();

                for (int i = 0 ; i < cloudSize ; ++i)
                {
                    const CT_Point &point = (*pointcloud)[(*pointcloudIndex)[i]];

                    if ((point.z >= _zmin) && (point.z <= _zmax))
                    {
                        newIndex->addIndex((*pointcloudIndex)[i]);

                        if (point.x < xmin) {xmin = point.x;}
                        if (point.x > xmax) {xmax = point.x;}
                        if (point.y < ymin) {ymin = point.y;}
                        if (point.y > ymax) {ymax = point.y;}
                        if (point.z < zmin) {zmin = point.z;}
                        if (point.z > zmax) {zmax = point.z;}
                     }
                }

                CT_StandardItemGroup* newGroup = new CT_StandardItemGroup(*groupModel, 0, outResult);

                CT_AxisAlignedBoundingBox* boundingBox = new CT_AxisAlignedBoundingBox(QVector3D(xmin, ymin, zmin), QVector3D(xmax, ymax, zmax));
                CT_Scene *newScene = new CT_Scene(sceneItemModel, 0, outResult, (CT_AbstractPointCloud*) pointcloud, newIndex, boundingBox);

                newScene->setAutoDeletePointCloud(false);
                newScene->setAutoDeletePointCloudIndex(true);

                newGroup->addItemDrawable(newScene);
                outResult->addGroup(newGroup);
            }
        }
    } else {
        qDebug() << "Probl�me d'it�rateur";
    }





}
