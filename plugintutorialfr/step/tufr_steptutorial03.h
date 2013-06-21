// Balises C++, �vitant les doublons d'includes
#ifndef TUFR_STEPTUTORIAL02_H
#define TUFR_STEPTUTORIAL02_H


// Inclusion de la classe parente des �tapes
#include "ct_step/abstract/ct_abstractstep.h"

// Une �tape doit h�riter de CT_AbstractStep
class TUFR_StepTutorial02 : public CT_AbstractStep
{
    // Macro Qt, donnant acc�s � certaines fonctionnalit�s
    // En l'occurence, permet de r�cup�rer le nom de l'�tape
    Q_OBJECT

public:

    // Constructeur de l'�tape
    TUFR_StepTutorial02(CT_StepInitializeData &dataInit);

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
    double _zmin;                         /*!< Z minimum de la placette a extraire*/
    double _zmax;                         /*!< Z maximum de la placette a extraire*/

};


#endif // TUFR_STEPTUTORIAL02_H
