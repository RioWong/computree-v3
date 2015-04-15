#ifndef PB_ACTIONMANUALINVENTORY_H
#define PB_ACTIONMANUALINVENTORY_H


#include "views/actions/pb_actionmanualinventoryoptions.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_circle.h"

#include <QRect>

class PB_ActionManualInventory : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT
public:

    PB_ActionManualInventory(QMap<const CT_Scene*, const CT_Circle*> *selectedDbh,
                             QMultiMap<const CT_Scene*, const CT_Circle*> *availableDbh,
                             QMap<QString, QStringList> *paramData,
                             QMap<const CT_Scene*, QMap<QString, QString> >  *suppAttributes);

    ~PB_ActionManualInventory();

    QString uniqueName() const;
    QString title() const;
    QString description() const;
    QIcon icon() const;
    QString type() const;

    void init();

    bool mousePressEvent(QMouseEvent *e);
    bool mouseMoveEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);
    bool wheelEvent(QWheelEvent *e);

    bool keyPressEvent(QKeyEvent *e);
    bool keyReleaseEvent(QKeyEvent *e);

    void draw(GraphicsViewInterface &view, PainterInterface &painter);
    void drawOverlay(GraphicsViewInterface &view, QPainter &painter);

    CT_AbstractAction* copy() const;

    void chooseForDbh(const QPoint &point);
    void chooseForAttributes(const QPoint &point);
    void selectActiveScene(const QPoint &point);
    void setAttributes(const CT_Scene *scene);

    void updateVisibility(CT_Scene *scene, CT_Circle *circle);
public slots:
    void visibilityChanged();

private:
    QPoint              _oldPos;
    Qt::MouseButtons    _buttonsPressed;
    CT_Circle*          _currentCircle;
    CT_Scene*           _currentScene;


    QMap<const CT_Scene*, const CT_Circle*>         *_selectedDbh;
    QMultiMap<const CT_Scene*, const CT_Circle*>    *_availableDbh;

    QMap<QString, QStringList>                      *_paramData;
    QMap<const CT_Scene*, QMap<QString, QString> >  *_suppAttributes;


    QColor              _othersScenesCirclesColor;
    QColor              _activeSceneCirclesColor;
    QColor              _othersCircleColor;
    QColor              _othersScenesCirclesLightColor;
    QColor              _currentCircleColor;
};


#endif // PB_ACTIONMANUALINVENTORY_H
