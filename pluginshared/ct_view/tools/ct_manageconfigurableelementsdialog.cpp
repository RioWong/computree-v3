#include "ct_manageconfigurableelementsdialog.h"
#include "ui_ct_manageconfigurableelementsdialog.h"

CT_ManageConfigurableElementsDialog::CT_ManageConfigurableElementsDialog(QList<CT_AbstractConfigurableElement *> &elements, QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CT_ManageConfigurableElementsDialog)
{
    ui->setupUi(this);

    this->setWindowTitle(title);

    _baseElements = elements;

    _listDialog = new CT_ElementListDialog(_baseElements, this);
}

CT_ManageConfigurableElementsDialog::~CT_ManageConfigurableElementsDialog()
{
    delete ui;
}

void CT_ManageConfigurableElementsDialog::on_pb_new_clicked()
{
    _listDialog->exec();
    CT_AbstractConfigurableElement* selectedElement = _listDialog->getSelectedElement();

    if (selectedElement != NULL)
    {
        CT_AbstractConfigurableElement* newElement = selectedElement->copy();
        newElement->configure();

        QListWidgetItem* newItem = new QListWidgetItem(ui->listWidget);
        newItem->setText(newElement->getName());
        newItem->setToolTip(newElement->getDetailledDescription());
        newItem->setSelected(true);
        newItem->setFlags(newItem->flags() | Qt::ItemIsUserCheckable);
        newItem->setCheckState(Qt::Checked);

        _addedElements.insert(newItem, newElement);
    }
}

void CT_ManageConfigurableElementsDialog::on_pb_modify_clicked()
{
    QList<QListWidgetItem*> list = ui->listWidget->selectedItems();
    if (list.isEmpty()) {return;}

    QListWidgetItem* item = list.first();
    CT_AbstractConfigurableElement* selectedElement = _addedElements.value(item, NULL);

    if (selectedElement != NULL)
    {
        selectedElement->configure();
        item->setText(selectedElement->getName());
    }
}

void CT_ManageConfigurableElementsDialog::on_pb_delete_clicked()
{
    QList<QListWidgetItem*> list = ui->listWidget->selectedItems();
    if (list.isEmpty()) {return;}

    QListWidgetItem *item = list.first();
    CT_AbstractConfigurableElement* selectedElement = _addedElements.take(item);

    if (selectedElement != NULL) {delete selectedElement;}
    if (item != NULL) {delete item;}
}
