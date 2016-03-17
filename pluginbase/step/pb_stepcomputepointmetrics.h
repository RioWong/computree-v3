#ifndef PB_STEPCOMPUTEPOINTMETRICS_H
#define PB_STEPCOMPUTEPOINTMETRICS_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_element/abstract/ct_abstractconfigurableelement.h"
#include "ct_filter/abstract/ct_abstractfilter.h"

// Inclusion of auto-indexation system
#include "ct_tools/model/ct_autorenamemodels.h"

class PB_StepComputePointMetrics: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepComputePointMetrics(CT_StepInitializeData &dataInit);
    ~PB_StepComputePointMetrics();

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

    SettingsNodeGroup* getAllSettings() const;
    bool setAllSettings(const SettingsNodeGroup *settings);

    /*! \brief Step copy
     * 
     * Step copy, used when a step is added by step contextual menu
     */
    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

protected:

    /*! \brief Input results specification
     * 
     * Specification of input results models needed by the step (IN)
     */
    void createInResultModelListProtected();

    /*! \brief Parameters DialogBox
     * 
     * DialogBox asking for step parameters
     */
    void createPostConfigurationDialog();

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
    CT_AutoRenameModels    _outMetrics_ModelName;
    QString                _inSceneDisplayableName;

    QList<CT_AbstractConfigurableElement *> _availableMetrics;
    QList<CT_AbstractConfigurableElement *> _selectedMetrics;

};

#endif // PB_STEPCOMPUTEPOINTMETRICS_H
