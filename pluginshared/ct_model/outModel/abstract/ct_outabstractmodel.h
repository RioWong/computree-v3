#ifndef CT_OUTABSTRACTMODEL_H
#define CT_OUTABSTRACTMODEL_H

#include "ct_model/abstract/ct_abstractmodel.h"

class QMutex;

/**
 * @brief Represent a OUPUT model. An CT_OutAbstractModel is just a class to know that is a OUPUT model.
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutAbstractModel : public CT_AbstractModel
{
    Q_OBJECT

public:
    /**
     * @brief Construct a OUTPUT model
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     */
    CT_OutAbstractModel(const QString &uniqueName,
                        const QString &description,
                        const QString &displayableName);
    ~CT_OutAbstractModel();

    /**
     * @brief Returns the original model if this model is a copy
     */
    CT_OutAbstractModel* originalModel() const;

    /**
     * @brief Returns true if at least one element that use this model is visible in a document
     */
    bool isVisible() const;

    /**
     * @brief Returns true if at least one element that use this model is visible in the document passed in parameter
     */
    bool isVisibleInDocument(const DocumentInterface *doc) const;

    /**
     * @brief Copy this model
     */
    virtual CT_OutAbstractModel* copy() const = 0;

    /**
     * @brief Returns the result where item is stocked. If it was a copy of a model the result will be found to the originalModel
     * @warning This will returns a no NULL element only if this model was set to an Item and the Item is added to a result
     *          or something in the tree structure of the result
     */
    CT_AbstractResult* result() const;

protected:

    /**
     * @brief Used by model to set the original model when a copy is made
     */
    void setOriginalModel(const CT_OutAbstractModel *model);

    friend class CT_AbstractItem;
    friend class CT_AbstractItemDrawable;

    /**
     * @brief Used by CT_AbstractItemDrawable or CT_AbstractItemAttribute to set the result of this model
     */
    void setResult(const CT_AbstractResult *res);

    /**
     * @brief Increment the counter that count the number of element (that use this model ) visible in the document passed in parameter
     *        Used by CT_AbstractItemDrawable
     */
    void incrementVisibleInDocument(const DocumentInterface *doc);

    /**
     * @brief Decrement the counter that count the number of element (that use this model ) visible in the document passed in parameter
     *        Used by CT_AbstractItemDrawable
     */
    void decrementVisibleInDocument(const DocumentInterface *doc);

private:
    CT_OutAbstractModel             *m_originalModel;
    QHash<DocumentInterface*, int>   m_visibleInDocuments;
    QMutex                          *m_originalModelMutex;
    CT_AbstractResult               *m_realResult;

private slots:
    void originalModelDestroyed();

signals:
    void visibilityChanged(bool v);
    void visibilityInDocumentChanged(const DocumentInterface *doc, bool v);
};

#endif // CT_OUTABSTRACTMODEL_H
