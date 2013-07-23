#ifndef INITEMWIDGET_H
#define INITEMWIDGET_H


#include "widgets/abstractinwidget.h"

namespace Ui {
    class INItemWidget;
}

class INItemWidget : public AbstractInWidget
{
    Q_OBJECT

public:

    enum ChoiceMode
    {
        C_OneIfMultiple,
        C_MultipleIfMultiple
    };

    enum FinderMode
    {
        F_Obligatory,
        F_Optional
    };


    explicit INItemWidget(AbstractInModel* model, QWidget *parent = 0);
    ~INItemWidget();

    bool isvalid();
    QString getItemType();
    QString getAlias();
    QString getDisplayableName();
    QString getDescription();

    INItemWidget::FinderMode getFinderMode();
    INItemWidget::ChoiceMode getChoiceMode();

private slots:
    void on_alias_textChanged(const QString &arg1);

private:
    Ui::INItemWidget *ui;
};

#endif // INITEMWIDGET_H
