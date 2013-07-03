#ifndef ABTRACTMODEL_H
#define ABTRACTMODEL_H

#include "qstandarditemmodel.h"
#include "widgets/abstractwidget.h"

class AbtractModel : public QStandardItem
{
public:

    enum ModelType
    {
        M_Result_IN,
        M_Group_IN,
        M_Item_IN,
        M_Result_OUT,
        M_Group_OUT,
        M_Item_OUT
    };

    AbtractModel();

    virtual AbtractModel::ModelType getModelType() = 0;

    virtual AbstractWidget* getWidget() = 0;
    virtual QString getName() = 0;
    QString getDef();

    virtual QString getAlias() {return getWidget()->getAlias();}
    virtual bool isValid() {return getWidget()->isvalid();}

    virtual QString getInModelsDefinition() = 0;
    void getChildrenInModelsDefinitions(QString &result);

    virtual QString getInComputeContent() = 0;
    void getChildrenInComputeContent(QString &result);



};

#endif // ABTRACTMODEL_H
