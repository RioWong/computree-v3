// Balises C++, �vitant les doublons d'includes
#ifndef TUFR_STEPTUTORIAL03_H
#define TUFR_STEPTUTORIAL03_H


// Inclusion de la classe parente des �tapes
#include "ct_step/abstract/ct_abstractstep.h"

/*! \class TUFR_StepTutorial03
 *
 * \brief Cette classe d�coupe une sc�ne de points en groupes paralellepipediques
 *
 * \param _nx Nombre de clusters selon x
 * \param _ny Nombre de clusters selon y
 * \param _nz Nombre de clusters selon z

 *
 * Mod�le IN : Aucun
 *  - CT_ResultGroup \n
 *      - CT_StandardItemGroup \n
 *          - CT_Scene \n
 *
 * Mod�le OUT : \n
 *  - CT_ResultGroup \n
 *      - CT_StandardItemGroup \n
 *          - CT_PointCluster \n
 *
 */

// Une �tape doit h�riter de CT_AbstractStep
class TUFR_StepTutorial03 : public CT_AbstractStep
{
    // Macro Qt, donnant acc�s � certaines fonctionnalit�s
    // En l'occurence, permet de r�cup�rer le nom de l'�tape
    Q_OBJECT

public:

    // Constructeur de l'�tape
    TUFR_StepTutorial03(CT_StepInitializeData &dataInit);

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

    // Dans la zone priv�e : les param�tres de l'�tapes (initilis�s dans le constructeur)
    int _nx;      /*!< Nombre de clusters selon x */
    int _ny;      /*!< Nombre de clusters selon y */
    int _nz;      /*!< Nombre de clusters selon z */

};


// Fin de balise
#endif // TUFR_STEPTUTORIAL03_H
