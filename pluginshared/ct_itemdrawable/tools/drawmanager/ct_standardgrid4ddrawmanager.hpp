#ifndef CT_STANDARDGRID4DDRAWMANAGER_HPP
#define CT_STANDARDGRID4DDRAWMANAGER_HPP

#include "ct_standardgrid4ddrawmanager.h"

#include "ct_itemdrawable/ct_grid4d.h"
#include <QDebug>

// Initialise static attributes
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_W_DISPLAY_VALUE = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigWDisplayValue();
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_WIRE_MODE_ENABLED = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigWireModeEnabled();
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_LOW_THRESHOLDS_ENABLED = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigLowThresholdsEnabled();
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_HIGH_THRESHOLDS_ENABLED = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigHighThresholdsEnabled();
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_LOW_THRESHOLDS_VALUE = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigLowThresholdValue();
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_HIGH_THRESHOLDS_VALUE = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigHighThresholdValue();
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_REDUCTION_COEF = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigReductionCoef();
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_TRANSPARENCY_ENABLED = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigEnableTransparency();
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_TRANSPARENCY_VALUE = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigTransparencyValue();
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_HIDE_PLANE_NB_XINF = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigXinf();
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_HIDE_PLANE_NB_XSUP = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigXsup();
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_HIDE_PLANE_NB_YINF = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigYinf();
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_HIDE_PLANE_NB_YSUP = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigYsup();
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_HIDE_PLANE_NB_ZINF = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigZinf();
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_HIDE_PLANE_NB_ZSUP = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigZsup();
template< typename DataT > const QString CT_StandardGrid4DDrawManager<DataT>::INDEX_CONFIG_SHOW_TRUES_ONLY = CT_StandardGrid4DDrawManager<DataT>::staticInitConfigShowTrueOnly();

template< typename DataT >
CT_StandardGrid4DDrawManager<DataT>::CT_StandardGrid4DDrawManager(QString drawConfigurationName)
    : CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager( drawConfigurationName.isEmpty() ? tr("4D Grid (%1)").arg(CT_TypeInfo::name<DataT>()) : drawConfigurationName )
{

}

template< typename DataT >
CT_StandardGrid4DDrawManager<DataT>::~CT_StandardGrid4DDrawManager()
{
}


template< typename DataT >
void CT_StandardGrid4DDrawManager<DataT>::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::draw(view, painter, itemDrawable);

    const CT_Grid4D<DataT> &item = dynamic_cast<const CT_Grid4D<DataT>&>(itemDrawable);

    // Getting the configuration values
    GLenum  drawingMode;
    bool    wireMode = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_WIRE_MODE_ENABLED).toBool();
    bool    uselowThresh = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_LOW_THRESHOLDS_ENABLED).toBool();
    bool    usehighThresh = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIGH_THRESHOLDS_ENABLED).toBool();
    double  lowThresh = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_LOW_THRESHOLDS_VALUE).toDouble();
    double  highThresh = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIGH_THRESHOLDS_VALUE).toDouble();
    double  reductionCoef = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_REDUCTION_COEF).toDouble();
    bool    useTransparency = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_TRANSPARENCY_ENABLED).toBool();
    int     transparencyValue = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_TRANSPARENCY_VALUE).toInt();

    size_t     ww  = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_W_DISPLAY_VALUE).toInt();
    size_t     nXinf = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_XINF).toInt();
    size_t     nXsup = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_XSUP).toInt();
    size_t     nYinf = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_YINF).toInt();
    size_t     nYsup = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_YSUP).toInt();
    size_t     nZinf = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_ZINF).toInt();
    size_t     nZsup = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_ZSUP).toInt();

    if (nXsup > item.xdim()) {nXsup = item.xdim();}
    if (nYsup > item.ydim()) {nXsup = item.ydim();}
    if (nZsup > item.zdim()) {nXsup = item.zdim();}

    if (!useTransparency || (transparencyValue > 255)) {transparencyValue = 255;}
    if (transparencyValue < 0) {transparencyValue = 0;}

    if ( wireMode ) {drawingMode = GL_LINE;}
    else            {drawingMode = GL_FILL;}

    if (!uselowThresh) {lowThresh =  (double)item.dataMin();}
    if (!usehighThresh) {highThresh =  (double)item.dataMax();}

    bool drawAsMap = true;
    if (lowThresh == highThresh) {drawAsMap = false;}

    QColor color = painter.getColor();

    // For each voxel of the grid
    double scaling = 240.0 / (highThresh - lowThresh);
    double offset = - (240*lowThresh)/(highThresh - lowThresh);

    size_t xdim = item.xdim();
    size_t ydim = item.ydim();
    size_t zdim = item.zdim();
    double demiResx = reductionCoef*item.xres() / 2.0;
    double demiResy = reductionCoef*item.yres() / 2.0;
    double demiResz = reductionCoef*item.zres() / 2.0;

    double xmin, ymin, zmin, xmax, ymax, zmax;

    // If w val > wwMax, do not display
    if ( ww <= item.wdim() )
    {
        // For each voxel of the grid
        for (size_t xx = nXinf ; xx < (xdim - nXsup) ; xx++)
        {
            for (size_t yy = nYinf ; yy < (ydim - nYsup)  ; yy++)
            {
                for (size_t zz = nZinf ; zz < (zdim - nZsup); zz++)
                {
                    size_t index;
                    if (item.index(ww, xx, yy, zz, index))
                    {
                        DataT data = item.valueAtIndex(index);

                        // Draw a cube if the value it contains is between the two thresholds
                        if ( data >= lowThresh && data <= highThresh )
                        {

                            if (drawAsMap && !itemDrawable.isSelected())
                            {
                                double h = (int) qRound((data*scaling) + offset);
                                painter.setColor( QColor::fromHsv(h,255,255,transparencyValue) );
                            } else {
                                painter.setColor(QColor(255,255,255));
                            }

                            xmin = item.getCellCenterX(xx) - demiResx;
                            ymin = item.getCellCenterY(yy) - demiResy;
                            zmin = item.getCellCenterZ(zz) - demiResz;
                            xmax = item.getCellCenterX(xx) + demiResx;
                            ymax = item.getCellCenterY(yy) + demiResy;
                            zmax = item.getCellCenterZ(zz) + demiResz;

                            painter.drawCube( xmin, ymin, zmin, xmax, ymax, zmax, GL_FRONT_AND_BACK, drawingMode );
                        }
                    }

                    else
                    {
                        qDebug() << "Probleme d'index (drawmanager)";
                    }
                }
            }
        }
    }

    painter.setColor(color);
}

template< typename DataT >
CT_ItemDrawableConfiguration CT_StandardGrid4DDrawManager<DataT>::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addNewConfiguration(staticInitConfigWDisplayValue(), "W fixe", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigWireModeEnabled(), "Mode filaire", CT_ItemDrawableConfiguration::Bool, true);
    item.addNewConfiguration(staticInitConfigLowThresholdsEnabled(), "Forcer limite basse", CT_ItemDrawableConfiguration::Bool, true);
    item.addNewConfiguration(staticInitConfigLowThresholdValue(), "Limite basse (forcée)", CT_ItemDrawableConfiguration::Double, 1 );
    item.addNewConfiguration(staticInitConfigHighThresholdsEnabled(), "Forcer limite haute", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(staticInitConfigHighThresholdValue(), "Limite haute (forcée)", CT_ItemDrawableConfiguration::Double, 100 );
    item.addNewConfiguration(staticInitConfigReductionCoef(), "Coef. de reduction", CT_ItemDrawableConfiguration::Double, 1);
    item.addNewConfiguration(staticInitConfigEnableTransparency(), "Activer transparence", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(staticInitConfigTransparencyValue(), "Valeur de transparence", CT_ItemDrawableConfiguration::Int, 100);
    item.addNewConfiguration(staticInitConfigXinf(), "Nb. Plans masqués X-", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigXsup(), "Nb. Plans masqués X+", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigYinf(), "Nb. Plans masqués Y-", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigYsup(), "Nb. Plans masqués Y+", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigZinf(), "Nb. Plans masqués Z-", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigZsup(), "Nb. Plans masqués Z+", CT_ItemDrawableConfiguration::Int, 0);

    return item;
}

// PROTECTED //
template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigWDisplayValue()
{
    return "A4DGD_WDV";
}

template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigWireModeEnabled()
{
    return "A4DGD_WME";
}

template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigLowThresholdsEnabled()
{
    return "A4DGD_LTE";
}

template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigHighThresholdsEnabled()
{
    return "A4DGD_HTE";
}

template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigLowThresholdValue()
{
    return "A4DGD_LTV";
}

template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigHighThresholdValue()
{
    return "A4DGD_HTV";
}

template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigReductionCoef()
{
    return "A4DGD_RDC";
}

template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigEnableTransparency()
{
    return "A4DGD_ETR";
}

template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigTransparencyValue()
{
    return "A4DGD_TRV";
}

template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigXinf()
{
    return "A4DGD_XIN";
}

template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigXsup()
{
    return "A4DGD_XSU";
}

template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigYinf()
{
    return "A4DGD_YIN";
}

template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigYsup()
{
    return "A4DGD_YSU";
}

template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigZinf()
{
    return "A4DGD_ZIN";
}

template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigZsup()
{
    return "A4DGD_ZSU";
}

template< typename DataT >
QString CT_StandardGrid4DDrawManager<DataT>::staticInitConfigShowTrueOnly()
{
    return "A4DGD_STO";
}

#endif // CT_STANDARDGRID4DDRAWMANAGER_HPP
