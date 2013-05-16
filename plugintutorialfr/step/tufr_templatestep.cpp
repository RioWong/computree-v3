#include "tufr_templatestep.h"

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

#include "qdebug.h" // pour afficher des messages � la console
#include "math.h"

// Alias des chaines de caract�res pour l'indexation des mod�les in et out
#define DEF_SearchInResult  "r"
#define DEF_SearchOutResult "r"

// Constructeur : initialisation des param�tres
TUFR_TemplateStep::TUFR_TemplateStep(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _parametre = 0;
}

// Description de l'�tape (tooltip du menu contextuel)
QString TUFR_TemplateStep::getStepDescription() const
{
    return "Description";
}

// M�thode de recopie de l'�tape
CT_VirtualAbstractStep* TUFR_TemplateStep::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette �tape
    return new TUFR_TemplateStep(dataInit);
}

//////////////////// PROTECTED //////////////////

// Cr�ation et affiliation des mod�les IN
void TUFR_TemplateStep::createInResultModelListProtected()
{
}

// Cr�ation et affiliation des mod�les OUT
void TUFR_TemplateStep::createOutResultModelListProtected()
{

}

// Cr�ation semi-automatique de la boite de dialogue de param�trage de l'�tape
void TUFR_TemplateStep::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble("Valeur du param�tre", "unit�", 0, 1000, 2, _parametre);
}

void TUFR_TemplateStep::compute()
{


    // PENSER A AJOUTER CETTE ETAPE AU STEPPLUGINMANAGER !!!!!

}
