#include "actions/pb_actionmodifyaffiliations.h"
#include "ct_global/ct_context.h"
#include "ct_math/ct_mathpoint.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPainter>
#include <QDebug>
#include <QVector3D>
#include <limits>


PB_ActionModifyAffiliations::PB_ActionModifyAffiliations(const QList<CT_AbstractSingularItemDrawable*> *sourceList,
                                                         const QList<CT_AbstractSingularItemDrawable*> *targetList,
                                                         QMap<CT_AbstractSingularItemDrawable*, CT_AbstractSingularItemDrawable*> *correspondances) : CT_AbstractActionForGraphicsView()
{
    m_status = 0;

    _symbolSize = 0.25;

    _sourceList = sourceList;
    _targetList = targetList;
    _correspondances = correspondances;

    for (int i = 0 ; i < _sourceList->size() ; i++)
    {
        CT_AbstractSingularItemDrawable* item = _sourceList->at(i);
        _itemCenters.insert(item, QVector3D(item->getCenterX(), item->getCenterY(), item->getCenterZ()));
    }

    for (int i = 0 ; i < _targetList->size() ; i++)
    {
        CT_AbstractSingularItemDrawable* item = _targetList->at(i);
        _itemCenters.insert(item, QVector3D(item->getCenterX(), item->getCenterY(), item->getCenterZ()));
    }

    _activeSource = NULL;
    _activeTarget = NULL;
}


QString PB_ActionModifyAffiliations::uniqueName() const
{
    return "PB_ActionModifyAffiliations";
}

QString PB_ActionModifyAffiliations::title() const
{
    return "Modify affiliations";
}

QString PB_ActionModifyAffiliations::description() const
{
    return "Permet de modifier des affiliations";
}

QIcon PB_ActionModifyAffiliations::icon() const
{
    return QIcon(":/icons/show_all.png");
}

QString PB_ActionModifyAffiliations::type() const
{
    return CT_AbstractAction::TYPE_MEASURE;
}

void PB_ActionModifyAffiliations::init()
{
    CT_AbstractActionForGraphicsView::init();

    m_status = 0;

    if(nOptions() == 0)
    {
        // create the option widget if it was not already created
        PB_ActionModifyAffiliationsOptions *option = new PB_ActionModifyAffiliationsOptions(this);

        // add the options to the graphics view
        graphicsView()->addActionOptions(option);

        connect(option, SIGNAL(parametersChanged()), this, SLOT(updateGraphics()));
        connect(option, SIGNAL(askForAffiliation()), this, SLOT(affiliate()));
        connect(option, SIGNAL(askForBreakingAffiliation()), this, SLOT(breakAffiliation()));

        // register the option to the superclass, so the hideOptions and showOptions
        // is managed automatically
        registerOption(option);

        document()->redrawGraphics();
    }
}

CT_AbstractSingularItemDrawable * PB_ActionModifyAffiliations::closetItemFromClickDirection(QMouseEvent *e,
                                                                                            const QList<CT_AbstractSingularItemDrawable*> *itemList)
{
    QVector3D origin, direction;
    GraphicsViewInterface *view = graphicsView();
    view->convertClickToLine(e->pos(), origin, direction);

    float minDist = std::numeric_limits<float>::max();
    CT_AbstractSingularItemDrawable* closestItem = NULL;

    int size = itemList->size();
    for (int i = 0 ; i < size ; i++)
    {
        CT_AbstractSingularItemDrawable* item = itemList->at(i);
        const QVector3D &center = _itemCenters.value(item);

        float distance = CT_MathPoint::distancePointLine<QVector3D>(center, direction, origin);
        if (distance < minDist)
        {
            minDist = distance;
            closestItem = item;
        }
    }

    return closestItem;
}


bool PB_ActionModifyAffiliations::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    return true;
}

bool PB_ActionModifyAffiliations::mousePressEvent(QMouseEvent *e)
{
    PB_ActionModifyAffiliationsOptions *option = (PB_ActionModifyAffiliationsOptions*)optionAt(0);

    if (option->selectionActivated())
    {
        if (e->buttons() & Qt::LeftButton)
        {
            _activeSource = closetItemFromClickDirection(e, _sourceList);
            document()->redrawGraphics();
        } else if (e->buttons() & Qt::RightButton)
        {
            _activeTarget = closetItemFromClickDirection(e, _targetList);
            document()->redrawGraphics();
        }
    }

    return false;
}

bool PB_ActionModifyAffiliations::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool PB_ActionModifyAffiliations::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool PB_ActionModifyAffiliations::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers()  & Qt::ControlModifier)
    {
        int sourceIndex = _sourceList->indexOf(_activeSource);
        if (e->delta()>0)
        {
            if (sourceIndex < (_sourceList->size() - 1))
            {
                _activeSource = _sourceList->at(sourceIndex + 1);
            }
        } else if (e->delta()<0)
        {
            if (sourceIndex > 0)
            {
                _activeSource = _sourceList->at(sourceIndex - 1);
            }
        }
        document()->redrawGraphics();
        return true;
    } else if (e->modifiers()  & Qt::ShiftModifier)
    {
        int targetIndex = _targetList->indexOf(_activeTarget);
        if (e->delta()>0)
        {
            if (targetIndex < (_targetList->size() - 1))
            {
                _activeTarget = _targetList->at(targetIndex + 1);
            }
        } else if (e->delta()<0)
        {
            if (targetIndex > 0)
            {
                _activeTarget = _targetList->at(targetIndex - 1);
            }
        }
        document()->redrawGraphics();
        return true;
    }

    return false;
}

bool PB_ActionModifyAffiliations::keyPressEvent(QKeyEvent *e)
{
    if((e->key() == Qt::Key_A)
            && !e->isAutoRepeat())
    {
        affiliate();
        return true;
    } else if((e->key() == Qt::Key_Z)
              && !e->isAutoRepeat())
    {
        breakAffiliation();
        return true;
    } else if((e->key() == Qt::Key_S)
                 && !e->isAutoRepeat())
       {
        PB_ActionModifyAffiliationsOptions *option = (PB_ActionModifyAffiliationsOptions*)optionAt(0);
        option->toggleSelection();
        return true;
       }
    return false;
}

bool PB_ActionModifyAffiliations::keyReleaseEvent(QKeyEvent *e)
{
    Q_UNUSED(e);
    return false;
}

void PB_ActionModifyAffiliations::updateGraphics()
{
    document()->redrawGraphics();
}

void PB_ActionModifyAffiliations::affiliate()
{
    if (_activeSource!=NULL && _activeTarget!=NULL)
    {
        _correspondances->remove(_activeTarget);

        QMutableMapIterator<CT_AbstractSingularItemDrawable*, CT_AbstractSingularItemDrawable*> it(*_correspondances);
        while (it.hasNext())
        {
            it.next();
            if (it.value() == _activeSource)
            {
                it.remove();
            }
        }

        _correspondances->insert(_activeTarget, _activeSource);

        document()->redrawGraphics();
    }
}

void PB_ActionModifyAffiliations::breakAffiliation()
{
    _correspondances->remove(_activeTarget);
    document()->redrawGraphics();
}

void PB_ActionModifyAffiliations::draw(GraphicsViewInterface &view, PainterInterface &painter)
{
    Q_UNUSED(view);

    PB_ActionModifyAffiliationsOptions *option = (PB_ActionModifyAffiliationsOptions*)optionAt(0);

    painter.save();

    if (option->sourceVisible())
    {
        for (int i = 0 ; i < _sourceList->size() ; i++)
        {
            CT_AbstractSingularItemDrawable* item = _sourceList->at(i);

            if (item == _activeSource)
            {
                painter.setColor(QColor(255, 0, 255));
            } else {
                painter.setColor(QColor(0, 0, 255));
            }


            if (!option->onlySelectedVisible() || item == _activeSource)
            {
                if (option->centersVisible())
                {
                    drawSourceItem(painter, _itemCenters.value(item));
                }

                if (option->itemsVisible())
                {
                    item->draw(view, painter);
                }
            }
        }
    }


    if (option->targetVisible())
    {
        for (int i = 0 ; i < _targetList->size() ; i++)
        {
            CT_AbstractSingularItemDrawable* item = _targetList->at(i);

            if (item == _activeTarget)
            {
                painter.setColor(QColor(255, 0, 0));
            } else {
                painter.setColor(QColor(0, 255, 0));
            }

            if (!option->onlySelectedVisible() || item == _activeTarget)
            {
                if (option->centersVisible())
                {
                    drawSourceItem(painter, _itemCenters.value(item));
                }

                if (option->itemsVisible())
                {
                    item->draw(view, painter);
                }
            }
        }
    }

    if (option->affiliationsLinesVisible())
    {
        QMapIterator<CT_AbstractSingularItemDrawable*, CT_AbstractSingularItemDrawable*> it(*_correspondances);
        while (it.hasNext())
        {
            it.next();

            if (it.key() == _activeTarget && it.value() == _activeSource)
            {
                painter.setColor(QColor(255, 0, 0));
            } else {
                painter.setColor(QColor(0, 255, 0));
            }

            if (!option->onlySelectedVisible() || it.key() == _activeTarget || it.value() == _activeSource)
            {

                drawAffiliation(painter, _itemCenters.value(it.key()), _itemCenters.value(it.value()));
            }
        }
    }

    painter.restore();
}

void PB_ActionModifyAffiliations::drawSourceItem(PainterInterface &painter, const QVector3D &position)
{
    painter.drawLine(position.x() - _symbolSize, position.y(), position.z(), position.x() + _symbolSize, position.y(), position.z());
    painter.drawLine(position.x(), position.y() - _symbolSize, position.z(), position.x(), position.y() + _symbolSize, position.z());
}

void PB_ActionModifyAffiliations::drawTargetItem(PainterInterface &painter, const QVector3D &position)
{
    painter.drawLine(position.x() - _symbolSize, position.y() - _symbolSize, position.z(), position.x() + _symbolSize, position.y() + _symbolSize, position.z());
    painter.drawLine(position.x() - _symbolSize, position.y() + _symbolSize, position.z(), position.x() + _symbolSize, position.y() - _symbolSize, position.z());
}

void PB_ActionModifyAffiliations::drawAffiliation(PainterInterface &painter, const QVector3D &source, const QVector3D &target)
{
    painter.drawLine(source.x(), source.y(), source.z(), target.x(), target.y(), target.z());
}

void PB_ActionModifyAffiliations::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
{
    Q_UNUSED(view)
    Q_UNUSED(painter)
}

CT_AbstractAction *PB_ActionModifyAffiliations::copy() const
{
    return new PB_ActionModifyAffiliations(_sourceList, _targetList, _correspondances);
}

