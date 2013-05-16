#ifndef TUFR_TEMPLATESTEP_H
#define TUFR_TEMPLATESTEP_H

#include "ct_step/abstract/ct_abstractstep.h"

class TUFR_TemplateStep : public CT_AbstractStep
{
    Q_OBJECT

public:

    // Constructeur de l'�tape
    TUFR_TemplateStep(CT_StepInitializeData &dataInit);

    // M�thode fournissant la description de l'�tape
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

    // param�tres de l'�tapes
    double _parametre;   /*!< param�tre */

};

#endif // TUFR_TEMPLATESTEP_H
