#ifndef ABTRACTMODEL_H
#define ABTRACTMODEL_H

#include "qstandarditemmodel.h"
#include "widgets/abstractinwidget.h"
#include "qset.h"

class AbstractInModel : public QStandardItem
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

    AbstractInModel();
    ~AbstractInModel();

    virtual AbstractInModel::ModelType getModelType() = 0;

    virtual AbstractInWidget* getWidget();
    virtual QString getName() = 0;
    QString getDef();

    virtual QString getAlias();
    virtual bool isValid();

    virtual QString getInModelsDefines();

    static QString getInModelsIncludes(QSet<QString> &list);
    virtual void getInModelsIncludesList(QSet<QString> &list) = 0;

    virtual QString getInModelsDefinition() = 0;
    void getChildrenInModelsDefinitions(QString &result);

    virtual QString getInModelsHierachy() = 0;
    void getChildrenInModelsHierachy(QString &result);
    virtual QString getInModelAddingCommand() = 0;

    virtual QString getInComputeContent() = 0;
    void getChildrenInComputeContent(QString &result);

    void onAliasChange();

protected:
    AbstractInWidget*  _widget;

};

#endif // ABTRACTMODEL_H
