#include "inresultwidget.h"
#include "ui_inresultwidget.h"

INResultWidget::INResultWidget(QWidget *parent) :
    AbstractWidget(parent),
    ui(new Ui::INResultWidget)
{
    ui->setupUi(this);
    ui->rb_standardresult->setChecked(true);
}

INResultWidget::~INResultWidget()
{
    delete ui;
}

bool INResultWidget::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}


QString INResultWidget::getAlias()
{
    return ui->alias->text();
}

QString INResultWidget::getDisplayableName()
{
    return ui->modelName->text();
}

QString INResultWidget::getDescription()
{
    return ui->modelDescription->toPlainText().replace("\n","\\n");
}

bool INResultWidget::getRecursive()
{
    return ui->cb_recursive->isChecked();
}


INResultWidget::ResultType  INResultWidget::getResultType()
{
    if (ui->rb_standardresult->isChecked())
    {
        return INResultWidget::R_StandardResult;
    } else {
        return INResultWidget::R_CopyResult;
    }
}
