#ifndef PB_STEPLOADTREEMAP_H
#define PB_STEPLOADTREEMAP_H

#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"

/*!
 * \class PB_StepLoadTreeMap
 * \ingroup Steps_PB
 * \brief <b>Charge deux sources de positions pour mise en correspondance.</b>
 *
 * No detailled description for this step
 *
 * \param _refFileName 
 * \param _transFileName 
 *
 */

class PB_StepLoadTreeMap: public CT_AbstractStepCanBeAddedFirst
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepLoadTreeMap(CT_StepInitializeData &dataInit);

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

public slots:
    void fileChanged();

signals:
    void updateComboBox(QStringList valuesList, QString value);

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

    void createOutResultModelListProtected();

    void compute();


private:

    // Step parameters
    QStringList _neededFields;

    QString _refFileName;
    QString _plotID;

    bool _refHeader;

    QString _refSeparator;

    QString _refDecimal;

    QLocale _refLocale;


    int _refSkip;

    QMap<QString, int> _refColumns;

    QStringList _plotsIds;


};

#endif // PB_STEPLOADTREEMAP_H
