// Balises C++, �vitant les doublons d'includes
#ifndef TUFR_STEPTUTORIAL04_H
#define TUFR_STEPTUTORIAL04_H

// Inclusion de la classe parente des �tapes
#include "ct_step/abstract/ct_abstractstep.h"

// Inclusion du syst�me d'indexation automatique
#include "ct_tools/model/ct_autorenamemodels.h"

/*! \class TUFR_StepTutorial04
 *
 * \brief Cette classe cr��e un CT_ReferencePoint pour chaque CT_PointCluster
 *
 * Mod�le IN : Aucun
 *  - CT_ResultGroup \n
 *      - CT_StandardItemGroup \n
 *          - CT_PointCluster \n
 *
 * Mod�le OUT : \n
 *  - CT_ResultGroup \n
 *      - CT_StandardItemGroup \n
 *          - cpy  CT_PointCluster \n
 *          - cpy+ CT_ReferencePoint \n
 *
 */

 // Une �tape doit h�riter de CT_AbstractStep
class TUFR_StepTutorial04 : public CT_AbstractStep
{
    // Macro Qt, donnant acc�s � certaines fonctionnalit�s
    // En l'occurence, permet de r�cup�rer le nom de l'�tape
    Q_OBJECT

public:

    // Constructeur de l'�tape
    TUFR_StepTutorial04(CT_StepInitializeData &dataInit);

    // M�thode fournissant la description de l'�tape (tooltip dans le menu contectuel)
    QString getStepDescription() const;

    // M�thode de copie de l'�tape, utilis�e lors de l'ajout d'une �tapes � partir du menu contextuel
    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

protected:

    // Sp�cifications des r�sultats d'entr�e souhait�s (IN)
    void createInResultModelListProtected();

    // Boite de saisie des param�tres de l'�tape
    void createPostConfigurationDialog();

    // Sp�cification des r�sultats de sortie cr��s par l'�tape (OUT)
    void createOutResultModelListProtected();

    // Etape de calcul, cr�ant les donn�es des r�sultats de sortie
    void compute();

private:

    // Attributs permettant l'indexation des cercles qu'on va ajouter
    CT_AutoRenameModels     _outRefPointModelName;

};


// Fin de balise
#endif // TUFR_STEPTUTORIAL04_H
