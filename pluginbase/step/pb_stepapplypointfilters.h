#ifndef PB_STEPAPPLYPOINTFILTERS_H
#define PB_STEPAPPLYPOINTFILTERS_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_element/abstract/ct_abstractconfigurableelement.h"
#include "ct_filter/abstract/ct_abstractfilter.h"
#include "ct_view/elements/ctg_configurableelementsselector.h"

// Inclusion of auto-indexation system
#include "ct_tools/model/ct_autorenamemodels.h"


class PB_StepApplyPointFilters: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepApplyPointFilters(CT_StepInitializeData &dataInit);
    ~PB_StepApplyPointFilters();

    /*! \brief Step description
     * 
     * Return a description of the step function
     */
    QString getStepDescription() const;

    /*! \brief Step detailled description
     * 
     * Return a detailled description of the step function
     */
    QString getStepDetailledDescription() const;

    /*! \brief Step URL
     * 
     * Return a URL of a wiki for this step
     */
    QString getStepURL() const;

    /*! \brief Step copy
     * 
     * Step copy, used when a step is added by step contextual menu
     */
    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);


    bool setAllSettings(const SettingsNodeGroup *settings);

    SettingsNodeGroup *getAllSettings() const;

protected:

    /*! \brief Input results specification
     * 
     * Specification of input results models needed by the step (IN)
     */
    void createInResultModelListProtected();

    bool postConfigure();


    /*! \brief Output results specification
     * 
     * Specification of output results models created by the step (OUT)
     */
    void createOutResultModelListProtected();

    /*! \brief Algorithm of the step
     * 
     * Step computation, using input results, and creating output results
     */
    void compute();

private:

    // Declaration of autoRenames Variables (groups or items added to In models copies)
    CT_AutoRenameModels    _outPoints_ModelName;

    QMap<CT_AbstractConfigurableElement*, CT_AutoRenameModels*>    _modelNames;

    QList<CT_AbstractConfigurableElement *> _availableFilters;
    QList<CT_AbstractConfigurableElement *> _selectedFilters;

};

#endif // PB_STEPAPPLYPOINTFILTERS_H
