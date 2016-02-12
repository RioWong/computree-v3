#ifndef PB_STEPCREATEREADERLIST_H
#define PB_STEPCREATEREADERLIST_H

#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"
#include "ct_reader/abstract/ct_abstractreader.h"


class PB_StepCreateReaderList: public CT_AbstractStepCanBeAddedFirst
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepCreateReaderList(CT_StepInitializeData &dataInit);

    ~PB_StepCreateReaderList();

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

    /*! \brief Input results specification
     * 
     * Specification of input results models needed by the step (IN)
     */
    void createInResultModelListProtected();

    void createPreConfigurationDialog();

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

    SettingsNodeGroup* getAllSettings() const;
    bool setAllSettings(const SettingsNodeGroup *settings);

private:

    // Step parameters
    QString                                         _readersListValue;
    QStringList                                     _filesList;

    QList<CT_AbstractReader*>                       _readersInstancesList;

    void initListOfAvailableReaders();
    void clear();

    QString getFormat(QString readerClassName) const;
    CT_AbstractReader* getReader(QString readerClassName) const;

};

#endif // PB_STEPCREATEREADERLIST_H
