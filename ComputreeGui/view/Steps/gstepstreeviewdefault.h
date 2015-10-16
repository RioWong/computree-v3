#ifndef GSTEPSTREEVIEWDEFAULT_H
#define GSTEPSTREEVIEWDEFAULT_H

#include <QTreeView>

/**
 * @brief Inherit from QTreeView to add a animated expand/collapse effect. (use by class GStepViewDefault)
 */
class GStepsTreeViewDefault : public QTreeView
{
    Q_OBJECT
public:
    explicit GStepsTreeViewDefault(QWidget *parent = 0);

signals:

private:
    QModelIndex m_indexToExpand;

private slots:
    void indexClicked(const QModelIndex &index);
    void expandLastIndex();
};

#endif // GSTEPSTREEVIEWDEFAULT_H
