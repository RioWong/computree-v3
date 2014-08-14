#include "ct_standardgrid3ddrawmanager.h"

/////////////////////////////////////////////////////////////////////
/// Specialisations for bool type ///////////////////////////////////
/////////////////////////////////////////////////////////////////////

template<>
void CT_StandardGrid3DDrawManager<bool>::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::draw(view, painter, itemDrawable);

    const CT_Grid3D<bool> &item = dynamic_cast<const CT_Grid3D<bool>&>(itemDrawable);

    // Getting the configuration values
    GLenum  drawingMode;
    bool    wireMode = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_WIRE_MODE_ENABLED).toBool();
    double  reductionCoef = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_REDUCTION_COEF).toDouble();
    bool    useTransparency = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_TRANSPARENCY_ENABLED).toBool();
    int     transparencyValue = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_TRANSPARENCY_VALUE).toInt();
    bool    showTrueOnly = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SHOW_TRUES_ONLY).toBool();

    int     nXinf = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_XINF).toInt();
    int     nXsup = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_XSUP).toInt();
    int     nYinf = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_YINF).toInt();
    int     nYsup = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_YSUP).toInt();
    int     nZinf = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_ZINF).toInt();
    int     nZsup = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_ZSUP).toInt();

    if (nXinf < 0) {nXinf = 0;}
    if (nYinf < 0) {nYinf = 0;}
    if (nZinf < 0) {nZinf = 0;}

    if (nXsup > item.xdim()) {nXsup = item.xdim();}
    if (nYsup > item.ydim()) {nXsup = item.ydim();}
    if (nZsup > item.zdim()) {nXsup = item.zdim();}

    if (!useTransparency || (transparencyValue > 255)) {transparencyValue = 255;}
    if (transparencyValue < 0) {transparencyValue = 0;}

    if ( wireMode ) {drawingMode = GL_LINE;}
    else            {drawingMode = GL_FILL;}

    bool drawAsMap = true;

    QColor color = painter.getColor();

    int xdim = item.xdim();
    int ydim = item.ydim();
    int zdim = item.zdim();
    float demiRes = reductionCoef*item.resolution() / 2.0;

    float xmin, ymin, zmin, xmax, ymax, zmax;

    // For each voxel of the grid
    for (size_t xx = nXinf ; xx < (xdim - nXsup) ; xx++)
    {
        for (size_t yy = nYinf ; yy < (ydim - nYsup)  ; yy++)
        {
            for (size_t zz = nZinf ; zz < (zdim - nZsup); zz++)
            {
                size_t index;

                if (item.index(xx, yy, zz, index))
                {
                    bool data = item.valueAtIndex(index);

                    if (data || !showTrueOnly)
                    {
                        if (drawAsMap && !itemDrawable.isSelected())
                        {
                            if (data)
                            {
                                painter.setColor(QColor(0,255,0));
                            } else {
                                painter.setColor(QColor(255,0,0));
                            }


                            xmin = item.getCellCenterX(xx) - demiRes;
                            ymin = item.getCellCenterY(yy) - demiRes;
                            zmin = item.getCellCenterZ(zz) - demiRes;
                            xmax = item.getCellCenterX(xx) + demiRes;
                            ymax = item.getCellCenterY(yy) + demiRes;
                            zmax = item.getCellCenterZ(zz) + demiRes;

                            painter.drawCube( xmin, ymin, zmin, xmax, ymax, zmax, GL_FRONT_AND_BACK, drawingMode );
                        }
                    }
                } else {
                    qDebug() << "Problème d'index (drawmanager)";
                }
            }
        }
        painter.setColor(color);
    }
}

template<>
CT_ItemDrawableConfiguration CT_StandardGrid3DDrawManager<bool>::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addNewConfiguration(staticInitConfigWireModeEnabled(), "Mode filaire", CT_ItemDrawableConfiguration::Bool, true);             // Draw the grid in wire mode
    item.addNewConfiguration(staticInitConfigReductionCoef(), "Coef. de reduction", CT_ItemDrawableConfiguration::Double, 1);
    item.addNewConfiguration(staticInitConfigShowTrueOnly(), "Valeurs TRUE seulement", CT_ItemDrawableConfiguration::Bool, 1);
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

