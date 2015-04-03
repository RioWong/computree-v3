#ifndef PB_ACTIONMODIFYCLUSTERSGROUPSOPTIONS_H
#define PB_ACTIONMODIFYCLUSTERSGROUPSOPTIONS_H

#include "ct_view/actions/abstract/ct_gabstractactionoptions.h"
#include "actions/pb_actionmodifyclustersgroups.h"

namespace Ui {
class PB_ActionModifyClustersGroupsOptions;
}

class PB_ActionModifyClustersGroupsOptions : public CT_GAbstractActionOptions
{
    Q_OBJECT

public:

    explicit PB_ActionModifyClustersGroupsOptions(const PB_ActionModifyClustersGroups *action);
    ~PB_ActionModifyClustersGroupsOptions();

    GraphicsViewInterface::SelectionMode selectionMode() const;

    bool isOnlyABChecked() const;

protected:
    QColor  _colorA;
    QColor  _colorB;

private:
    Ui::PB_ActionModifyClustersGroupsOptions *ui;

private slots:
    void on_buttonGroupSelection_buttonReleased(int id);
    void on_buttonGroupType_buttonReleased(int id);
    void on_buttonGroupMode_buttonReleased(int id);

    void on_pb_SetSceneA_clicked();
    void on_pb_SetSceneB_clicked();

    void on_pb_ColorA_clicked();
    void on_pb_ColorB_clicked();

    void on_cb_ShowOnlyAAndB_toggled(bool checked);

public slots:

    void setSelectionMode(GraphicsViewInterface::SelectionMode mode);

signals:

    void selectionModeChanged(GraphicsViewInterface::SelectionMode mode);
    void selectPositionA();
    void selectPositionB();
    void setColorA(QColor color);
    void setColorB(QColor color);
    void onlyABChanged();
};

#endif // PB_ACTIONMODIFYCLUSTERSGROUPSOPTIONS_H
