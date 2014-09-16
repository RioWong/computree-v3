#ifndef DM_SORTFILTERMATHPROXYMODEL_H
#define DM_SORTFILTERMATHPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "muParser/muParser.h"

class DM_SortFilterMathProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit DM_SortFilterMathProxyModel(QObject *parent = 0);

    void setVariableInMathExpression(const QString &var);
    bool canSetMathExpression(const QString &expression, bool verbose = false);
    bool setMathExpression(const QString &expression);

    QString variableInMathExpression() const;

    void setAcceptRows(bool enable, bool invalidFilter = true);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
    mutable mu::Parser  m_parser;
    QString             m_mathExpression;
    QString             m_var;
    int                 m_acceptRow;
};

#endif // DM_SORTFILTERMATHPROXYMODEL_H
