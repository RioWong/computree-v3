#ifndef WIDGETTEXTLINE_H
#define WIDGETTEXTLINE_H

#include "parameters/widgets/abstractparameterwidget.h"

namespace Ui {
    class WidgetTextLine;
}

class WidgetTextLine : public AbstractParameterWidget
{
    Q_OBJECT

public:

    explicit WidgetTextLine(AbstractParameter* model, QWidget *parent = 0);
    ~WidgetTextLine();

    bool isvalid();
    QString getAlias();
    QString getBeforeText();
    QString getAfterText();
    QString getCheckboxText();
    bool getDefaultValue();
    QString getDescription();

private slots:
    void on_alias_textChanged(const QString &arg1);

private:
    Ui::WidgetTextLine *ui;
};

#endif // WIDGETTEXTLINE_H
