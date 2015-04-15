#ifndef PB_ACTIONMANUALINVENTORYOPTIONS_H
#define PB_ACTIONMANUALINVENTORYOPTIONS_H

#include "ct_view/actions/abstract/ct_gabstractactionoptions.h"

class PB_ActionManualInventory;

namespace Ui {
class PB_ActionManualInventoryOptions;
}

class PB_ActionManualInventoryOptions : public CT_GAbstractActionOptions
{
    Q_OBJECT

public:

    explicit PB_ActionManualInventoryOptions(const PB_ActionManualInventory *action);
    ~PB_ActionManualInventoryOptions();

    bool shouldAutoCenterCamera();

    bool isSelectModeSelected();
    bool isDbhModeSelected();
    bool isAttributesModeSelected();

    bool isShowDataChecked();
    bool isShowActiveCirclesChecked();
    bool isShowOtherCirclesChecked();
    bool isShowActiveSceneChecked();
    bool isShowOtherScenesChecked();

private:
    Ui::PB_ActionManualInventoryOptions *ui;

signals:
    void modeChanged();
    void visibilityChanged();

private slots:

    void on_rb_select_toggled(bool checked);
    void on_rb_dbh_toggled(bool checked);
    void on_rb_attributes_toggled(bool checked);

    void on_cb_showData_toggled(bool checked);
    void on_cb_activeCircles_toggled(bool checked);
    void on_cb_otherCircles_toggled(bool checked);
    void on_cb_activeScene_toggled(bool checked);
    void on_cb_otherScenes_toggled(bool checked);
};

#endif // PB_ACTIONMANUALINVENTORYOPTIONS_H
