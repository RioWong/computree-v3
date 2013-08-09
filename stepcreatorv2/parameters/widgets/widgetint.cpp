#include "parameters/widgets/widgetint.h"
#include "ui_widgetint.h"
#include "parameters/abstractparameter.h"
#include "tools.h"

WidgetInt::WidgetInt(AbstractParameter* model, QWidget *parent) :
    AbstractParameterWidget(model, parent),
    ui(new Ui::WidgetInt)
{
    ui->setupUi(this);
    setFocusProxy(ui->alias);
}

WidgetInt::~WidgetInt()
{
    delete ui;
}

bool WidgetInt::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString WidgetInt::getAlias()
{
    return ui->alias->text();
}

QString WidgetInt::getBeforeText()
{
    return ui->libBefore->text();
}

QString WidgetInt::getAfterText()
{
    return ui->libAfter->text();
}

QString WidgetInt::getCheckboxText()
{
    return ui->libCheckbox->text();
}

bool WidgetInt::getDefaultValue()
{
    return ui->defaultValue->isChecked();
}

QString WidgetInt::getDescription()
{
    return ui->description->toPlainText().replace("\n","\\n");
}

void WidgetInt::on_alias_textChanged(const QString &arg1)
{
    if (arg1.size()>0)
    {
        if (arg1.at(0)!='_')
        {
            ui->alias->setText("_" + arg1);
        }
    }
    _model->onAliasChange();
}
