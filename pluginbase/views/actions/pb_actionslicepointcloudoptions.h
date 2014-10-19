#ifndef PB_ACTIONSLICEPOINTCLOUDOPTIONS_H
#define PB_ACTIONSLICEPOINTCLOUDOPTIONS_H

#include "ct_view/actions/abstract/ct_gabstractactionoptions.h"

class PB_ActionSlicePointCloud;

namespace Ui {
class PB_ActionSlicePointCloudOptions;
}

class PB_ActionSlicePointCloudOptions : public CT_GAbstractActionOptions
{
    Q_OBJECT

public:

    explicit PB_ActionSlicePointCloudOptions(const PB_ActionSlicePointCloud *action);
    ~PB_ActionSlicePointCloudOptions();

    double getThickness() const;
    double getSpacing() const;
    double getIncrement() const;

    void setThickness(double t) const;
    void setSpacing(double s) const;
    void setIncrement(double i) const;


private:
    Ui::PB_ActionSlicePointCloudOptions *ui;

signals:
    void parametersChanged();

};

#endif // PB_ACTIONSLICEPOINTCLOUDOPTIONS_H
