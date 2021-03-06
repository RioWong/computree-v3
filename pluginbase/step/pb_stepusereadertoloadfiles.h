#ifndef PB_STEPUSEREADERTOLOADFILES_H
#define PB_STEPUSEREADERTOLOADFILES_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_tools/model/ct_autorenamemodels.h"
#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"
#include "ct_reader/tools/ct_readerresultaddingtools.h"

class PB_StepUseReaderToLoadFiles: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepUseReaderToLoadFiles(CT_StepInitializeData &dataInit);

    ~PB_StepUseReaderToLoadFiles();

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


protected:

    void createPreConfigurationDialog();

    /*! \brief Input results specification
     * 
     * Specification of input results models needed by the step (IN)
     */
    void createInResultModelListProtected();

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

    bool                                    _conditionnal;
    CT_ReaderResultAddingTools              m_readerAddingTools;
    int                                     m_readerAutoIndex;
    int                                     m_totalReaderProgress;
    float                                   m_currentReaderProgress;

private slots:
    void readerProgressChanged(int p);
};

#endif // PB_STEPUSEREADERTOLOADFILES_H
