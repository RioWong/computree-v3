// Inclusion du fichier d'ent�te
#include "tufr_steptutorial01.h"

// Inclusion des mod�les de r�sultats in et out
#include "ct_result/model/inModel/ct_inresultmodelnotneedinputresult.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

// Inclusion de la classe de r�sultat standard
#include "ct_result/ct_resultgroup.h"

// Inclusion des CT_ItemDrawables utiles dans l'�tape
#include "ct_itemdrawable/ct_scene.h"

#include "qdebug.h" // pour afficher des messages � la console

// Alias des chaines de caract�res pour l'indexation du mod�le out
#define DEF_SearchOutResult "r"

// Constructeur : appel du constructeur de la classe m�re
//                Initialisation des param�tres (valeurs par d�faut)
TUFR_StepTutorial01::TUFR_StepTutorial01(CT_StepInitializeData &dataInit) : CT_AbstractStepCanBeAddedFirst(dataInit)
{
    _intParam = 10;
    _doubleParam = 50.6;
    _boolParam = true;
    _listParam = "choix 2";
    _choiceParam = 1;
    _fileNameParam = "";
}

// Description de l'�tape (tooltip du menu contextuel)
QString TUFR_StepTutorial01::getStepDescription() const
{
    return "Etape de d�monstration des types de param�tres";
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
    CT_InResultModelNotNeedInputResult *resultModel = new CT_InResultModelNotNeedInputResult();

    // Ajout du mod�le de r�sultat dans cette �tape
    addInResultModel(resultModel);
}

// Cr�ation et affiliation des mod�les OUT
void TUFR_StepTutorial01::createOutResultModelListProtected()
{
    // D�claration et cr�ation du mod�le de r�sultat  
    // Le 1er param�tre est l'alias d'indexation du mod�le de r�sultat
    // Le 2ieme param�tre est le mod�le du groupe racine
    // Le 3ieme param�tre donne un nom au r�sultat
    CT_OutResultModelGroup *resultModel = new CT_OutResultModelGroup(DEF_SearchOutResult, NULL, "R�sultat vide");

    // Ajout du mod�le de r�sultat s�quenciellement dans cette �tape
    addOutResultModel(resultModel);
}

// Cr�ation semi-automatique de la boite de dialogue de param�trage de l'�tape
void TUFR_StepTutorial01::createPostConfigurationDialog()
{
    // Pas de boite de dialogue
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    // Ajout de contr�les unitaires de param�trages � la boite (un par param�tre en g�n�ral)
    // Param�tre prenant une valeur enti�re
    configDialog->addInt("Param�tre entier", "unit�", 0, 100, _intParam);

    // Param�tre prenant une valeur double
    configDialog->addDouble("Param�tre double", "unit�", 0, 100, 2, _doubleParam);

    // Param�tre prenant une valeur bool�enne
    configDialog->addBool("Param�tre bool�en", "labelAfter", "labelCheckBox", _boolParam);

    // Param�tre prenant une valeur parmi une liste de choix
    // 1) cr�ation de la liste des choix possibles
    QStringList liste;
    liste.append("choix 1");
    liste.append("choix 2");
    liste.append("choix 3");
    // 2) ajout de la liste de choix � la boite de dialogue
    configDialog->addStringChoice("Param�tre choix dans une liste", "labelAfter", liste, _listParam);

    // Param�tre prenant une valeur de choix
    // Cr�ation du groupe de valeurs exclusives dans la boite de dialogue (non visible)
    CT_ButtonGroup &buttonGroup = configDialog->addButtonGroup(_choiceParam);

    // Cr�ation des items s�l�ctionnables de fa�on exclusive (visibles)
    configDialog->addExcludeValue("Valeur exclusive 1", "labelAfter", "labelRadioButton", buttonGroup, 1);
    configDialog->addExcludeValue("Valeur exclusive 2", "labelAfter", "labelRadioButton", buttonGroup, 2);
    configDialog->addExcludeValue("Valeur exclusive 3", "labelAfter", "labelRadioButton", buttonGroup, 3);

    // Param�tre prenant une valeur "nom de fichier"
    configDialog->addFileChoice("Param�tre nom de Fichier", CT_FileChoiceButton::OneExistingFile, "Tout fichier (*.*)",_fileNameParam);
}

void TUFR_StepTutorial01::compute()
{
    // Aucune action, sauf affichage des valeurs de param�tres choisies � la console

    qDebug();
    qDebug() << "Param�tre entier = " << _intParam;
    qDebug() << "Param�tre double = " << _doubleParam;
    qDebug() << "Param�tre bool�enne = " << _boolParam;
    qDebug() << "Param�tre choix dans une liste = " << _listParam;
    qDebug() << "Param�tre valeur exclusive = " << _choiceParam;
    qDebug() << "Param�tre Nom de fichier = " << _fileNameParam;

}
