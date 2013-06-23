// Inclusion du fichier d'ent�te
#include "tufr_steptutorial05.h"

// Inclusion des mod�les de r�sultats in et out, PERMETTANT LA COPIE
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"

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

// Inclusion des item utiles
#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_circle.h"

// Alias des chaines de caract�res pour l'indexation des mod�les in et out
#define DEF_SearchInResult  "r"
#define DEF_SearchInPointCloud   "pc"

#define DEF_SearchOutResult  "r"

TUFR_StepTutorial05::TUFR_StepTutorial05(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    // pas de param�tres
}

// Description de l'�tape (tooltip du menu contextuel)
QString TUFR_StepTutorial05::getStepDescription() const
{
    return "Ajuste des cercles sur des groupes de points";
}

// M�thode de recopie de l'�tape
CT_VirtualAbstractStep* TUFR_StepTutorial05::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette �tape
    return new TUFR_StepTutorial05(dataInit);
}

//////////////////// PROTECTED //////////////////

// Cr�ation et affiliation des mod�les IN
void TUFR_StepTutorial05::createInResultModelListProtected()
{
    // D�claration et cr�ation du mod�le de groupe racine
    CT_InOneOrMoreGroupModel *groupModel;
    groupModel = new CT_InOneOrMoreGroupModel();

    // D�claration et cr�ation du mod�le d'item groupe de points
    CT_InStandardItemDrawableModel *pointCloudItemModel;
    pointCloudItemModel = new CT_InStandardItemDrawableModel(CT_AbstractItemDrawableWithPointCloud::staticGetType(),
                             CT_InStandardItemDrawableModel::C_ChooseOneIfMultiple,
                             DEF_SearchInPointCloud,
                             CT_InStandardItemDrawableModel::F_IsObligatory,
                             "Groupe de points",
                             "Objet h�ritant de CT_AbstractItemDrawableWithPointCloud");

    // On ajoute le mod�le d'item (nuage de points) au mod�le du groupe racine
    groupModel->addItem(pointCloudItemModel);

    // D�claration et cr�ation du mod�le de r�sultat
    CT_InResultModelGroup *resultModel;
    resultModel = new CT_InResultModelGroup(groupModel,
                                            DEF_SearchInResult,
                                            false,
                                            "Un r�sultat avec une/des groupes de points",
                                            "R�sultat avec un/des groupes de points");

    // Ajout du mod�le de r�sultat s�quenciellement dans cette �tape
    // En r�alit� cette m�thode g�n�re aussi un resulat auquel est attach� ce mod�le
    // Les addInResultModel susccessifs, ajoutent des r�sultats dans l'ordre � la InResultList
    addInResultModel(resultModel);
}

// Cr�ation et affiliation des mod�les OUT
void TUFR_StepTutorial05::createOutResultModelListProtected()
{
    // on r�cup�re le r�sultat mod�le d'entr�e � copier d�fini dans "createInResultModelListProtected()"
    CT_InResultModelGroupToCopy *intResModelToCopy = (CT_InResultModelGroupToCopy*)getInResultModel(DEF_SearchInResult);

    // on r�cup�re toutes les possibilit�s de RESULTATS que l'utilisateur a d�fini
    // ici il n'y en aobligatoirement une seule
    QList<CT_OutResultModelGroupToCopyPossibility*> copyList = intResModelToCopy->getOutResultModelGroupsSelectedToCopy();

    // On v�rifie qu'il y a au moins une possibilit� (on sait jamais ...)
    if (!copyList.isEmpty())
    {
        // On r�cup�re la possibilit� choisie
        CT_OutResultModelGroupToCopyPossibility *outResModelToCopy = copyList.first();

        // on r�cup�re le r�sultat mod�le de sortie � copier/modifier
        CT_OutResultModelGroup *outResModelModif = outResModelToCopy->outModelForModification();
        const CT_OutResultModelGroup *outResModelSearch = outResModelToCopy->outModelForSearch();

        // on r�cup�re le mod�le du nuage de point d�fini dans "createInResultModelListProtected()"
        // pour la recherche
        CT_InAbstractItemDrawableModel *inPointCloudModel = (CT_InAbstractItemDrawableModel*)getInModel(*outResModelSearch, DEF_SearchInPointCloud);

        // on recherche tous les mod�les d'items du type nuage de points qui ont �t� s�lectionn�s
        // par l'utilisateur
        if(outResModelModif->recursiveBeginIterateItemsModel(*inPointCloudModel))
        {
            DEF_CT_AbstractGroupModelOut *outModel;

            // un if et non un while car on a d�fini C_ChooseOneIfMultiple
            // il ne peut donc y avoir qu'un seul mod�le choisi pour le nuage de point
            if((outModel = outResModelModif->recursiveNextItemModel()) != NULL)
            {
                // un nouveau mod�le pour les cercles SANS NOM puisqu'il sera d�fini automatiquement
                CT_OutStandardItemDrawableModel *newItemModel = new CT_OutStandardItemDrawableModel(new CT_Circle(), "", "Cercle", "Cercle ajust�");

                // On passe le mod�le de cercle au mod�le du groupe contenant le mod�le du nuage de points
                // on passe aussi un objet de type CT_AutoRenameModels permettant de renommer l'item au cas o� le nom
                // du mod�le existe d�j� (obligatoire dans ce cas puisqu'on ne connait pas les noms
                // des mod�les du r�sultat que l'on copie et il risque d'y avoir conflit)
                outModel->addItem(newItemModel, _outCircleModelName);
            }
        }

        // on cr�e un nouveau r�sultat de sortie contenant le r�sultat mod�le modifi�
        addOutResultModel(new CT_OutResultModelGroupCopy(DEF_SearchOutResult, outResModelToCopy));
    }
}

// Cr�ation semi-automatique de la boite de dialogue de param�trage de l'�tape
void TUFR_StepTutorial05::createPostConfigurationDialog()
{
    // Pas de boite de dialogue
}

void TUFR_StepTutorial05::compute()
{
    // on r�cup�re le r�sultat copi� (r�sultat de sortie donc)
    CT_ResultGroup *outRes = getOutResultList().first();

    // ainsi que son mod�le
    CT_OutAbstractResultModel *outResModel = outRes->model();

    // on r�cup�re le mod�le d'entr�e qu'on avait ajout�
    CT_InResultModelGroupToCopy *intResModel = (CT_InResultModelGroupToCopy*)getInResultModel(DEF_SearchInResult);

    // Afin de r�cup�rer le mod�le de sortie correspondant
    // Ici il n'y a qu'une possiblit�, donc un seul mod�le, d'o� le .first()
    const CT_OutResultModelGroup *outModelForSearchInModel = intResModel->getOutResultModelForSearchInModel().first();

    // et qu'on utilise ici pour rechercher le mod�le d'entr�e du nuage de point
    CT_InAbstractItemDrawableModel *inPointCloudModel = (CT_InAbstractItemDrawableModel*)getInModel(*outModelForSearchInModel, DEF_SearchInPointCloud);

    // on utilise le mod�le du r�sultat de sortie pour rechercher le mod�le de sortie du Cercle.
    // En utilisant le nom automatique g�n�r� lors de l'ajout (grace au CT_AutoRenameModels)
    CT_OutAbstractModel *outCircleModel = getOutModel(*outResModel, _outCircleModelName.completeName());

    /**************************************************/

    // on va rechercher tous les groupes contenant des nuages de points (qui ont �t� choisi par l'utilisateur)
    if(outRes->recursiveBeginIterateItems(*inPointCloudModel))
    {
        CT_AbstractItemGroup *group;

        // pour chaque groupe contenant un nuage de point
        while((group = outRes->recursiveNextItem()) != NULL && (!isStopped()))
        {
            group->beginIterateItems(*inPointCloudModel);

            // on r�cup�re le groupe de point
            const CT_AbstractItemDrawableWithPointCloud *item = (const CT_AbstractItemDrawableWithPointCloud*)group->nextItem();

            // on cr�� le cercle math�matique
            CT_CircleData *cData = CT_CircleData::staticCreateZAxisAlignedCircleDataFromPointCloud(*item->getPointCloud(),
                                                                                                   *item->getPointCloudIndex(),                                                                                                   item->getCenterZ());

            // et on ajoute un itemDrawable cercle
            if(cData != NULL)
            {
                group->addItemDrawable(new CT_Circle(outCircleModel, item->id(), outRes, cData));
            }
        }
    }
}
