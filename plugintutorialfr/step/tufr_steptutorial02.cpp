// Inclusion du fichier d'ent�te
#include "tufr_steptutorial01.h"

// Inclusion des mod�les de r�sultats in et out
#include "ct_result/model/inModel/ct_inresultmodelnotneedinputresult.h"
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
#include "ct_pointcloudstdvector.h"
#include "ct_element/ct_axisalignedboundingbox.h"

#include "qdebug.h" // pour afficher des messages � la console

#include "math.h"

// Alias des chaines de caract�res pour l'indexation du mod�le out
#define DEF_SearchOutResult "r"
#define DEF_SearchOutGroup  "g"
#define DEF_SearchOutScene  "sc"

// Constructeur : appel du constructeur de la classe m�re
//                Initialisation des param�tres (valeurs par d�faut)
TUFR_StepTutorial01::TUFR_StepTutorial01(CT_StepInitializeData &dataInit) : CT_AbstractStepCanBeAddedFirst(dataInit)
{
    _n = 10000;
    _xmin = -10;
    _xmax = 10;
    _ymin = -10;
    _ymax = 10;
    _zmin = -1;
    _zmax = 20;
}

// Description de l'�tape (tooltip du menu contextuel)
QString TUFR_StepTutorial01::getStepDescription() const
{
    return "G�n�re une sc�ne";
}

// M�thode de recopie de l'�tape
CT_VirtualAbstractStep* TUFR_StepTutorial01::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette �tape
    return new TUFR_StepTutorial01(dataInit);
}

//////////////////// PROTECTED //////////////////

// Cr�ation et affiliation des mod�les IN
void TUFR_StepTutorial01::createInResultModelListProtected()
{
    // D�claration et cr�ation du mod�le de r�sultat : Pas de r�sultat
    CT_InResultModelNotNeedInputResult *resultModel;
    resultModel = new CT_InResultModelNotNeedInputResult();

    // Ajout du mod�le de r�sultat dans cette �tape
    addInResultModel(resultModel);
}

// Cr�ation et affiliation des mod�les OUT
void TUFR_StepTutorial01::createOutResultModelListProtected()
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
                             "Sc�ne g�n�r�e",
                             "Sc�ne g�n�r�e");

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
                                             "GeneratedPlot",
                                             "Sc�ne g�n�r�e",
                                             "Sc�ne g�n�r�e");

    // Ajout du mod�le de r�sultat s�quenciellement dans cette �tape
    // En r�alit� cette m�thode g�n�re aussi un resulat auquel est attach� ce mod�le
    // Les addOutResultModel susccessifs, ajoutent des r�sultats dans l'ordre � la OutResultList
    addOutResultModel(resultModel);
}

// Cr�ation semi-automatique de la boite de dialogue de param�trage de l'�tape
void TUFR_StepTutorial01::createPostConfigurationDialog()
{
    // Pas de boite de dialogue
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    // Ajout de contr�les unitaires de param�trages � la boite (un par param�tre en g�n�ral)
    configDialog->addInt("Nombre de points al�atoires � g�n�rer", "", 1, 1000000, _n);
    configDialog->addDouble("X minimum :", "m", -10000, 10000, 2, _xmin);
    configDialog->addDouble("X maximum :", "m", -10000, 10000, 2, _xmax);
    configDialog->addDouble("Y minimum :", "m", -10000, 10000, 2, _ymin);
    configDialog->addDouble("Y maximum :", "m", -10000, 10000, 2, _ymax);
    configDialog->addDouble("Z minimum :", "m", -10000, 10000, 2, _zmin);
    configDialog->addDouble("Z maximum :", "m", -10000, 10000, 2, _zmax);
}

void TUFR_StepTutorial01::compute()
{
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

    CT_PointCloudStdVector* newCloud = new CT_PointCloudStdVector(_n);
    CT_PointCloudIndexVector *newIndex = new CT_PointCloudIndexVector(_n);

    double tmp;
    if (_xmin > _xmax) {tmp = _xmin; _xmin = _xmax; _xmax = tmp;}
    if (_ymin > _ymax) {tmp = _ymin; _ymin = _ymax; _ymax = tmp;}
    if (_zmin > _zmax) {tmp = _zmin; _zmin = _zmax; _zmax = tmp;}

    for (int i = 0 ; i < _n ; i++)
    {
        CT_Point& point = newCloud->operator [](i);
        newIndex->operator [](i) = i;
        point.x = (rand()/(double)RAND_MAX ) * (_xmax-_xmin) + _xmin;
        point.y = (rand()/(double)RAND_MAX ) * (_ymax-_ymin) + _ymin;
        point.z = (rand()/(double)RAND_MAX ) * (_zmax-_zmin) + _zmin;
        point.intensity = 50000;

    }


    CT_StandardItemGroup* newGroup = new CT_StandardItemGroup(*groupModel, 0, outResult);
    CT_AxisAlignedBoundingBox* boundingBox = new CT_AxisAlignedBoundingBox(QVector3D(_xmin, _ymin, _zmin), QVector3D(_xmax, _ymax, _zmax));
    CT_Scene *newScene = new CT_Scene(sceneItemModel, 0, outResult, newCloud, newIndex, boundingBox);
    newScene->setAutoDeletePointCloud(true);
    newScene->setAutoDeletePointCloudIndex(true);

    newGroup->addItemDrawable(newScene);
    outResult->addGroup(newGroup);

}
