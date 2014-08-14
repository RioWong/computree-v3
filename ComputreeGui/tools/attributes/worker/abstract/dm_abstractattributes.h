#ifndef DM_ABSTRACTATTRIBUTES_H
#define DM_ABSTRACTATTRIBUTES_H

#include <QString>
#include "tools/worker/dm_abstractworker.h"

class GDocumentViewForGraphics;
class IAttributes;

class DM_AbstractAttributes : public DM_AbstractWorker
{
    Q_OBJECT
public:
    DM_AbstractAttributes();
    virtual ~DM_AbstractAttributes();

    /**
     * @brief Set a displayable name
     */
    void setDisplayableName(const QString &name);

    /**
     * @brief Get the displayable name
     */
    QString displayableName() const;

    /**
     * @brief Get the Attributes
     */
    IAttributes* abstractAttributes() const;

    /**
     * @brief Set if he must be displayed alone
     */
    void setDisplayAlone(bool val);

    /**
     * @brief Must be displayed alone ?
     */
    bool isDisplayedAlone() const;

    /**
     * @brief Set the document where apply the attributes
     */
    void setDocument(const GDocumentViewForGraphics *doc);

    /**
     * @brief Set the document where the attributes must be applied
     */
    GDocumentViewForGraphics* document() const;

public slots:

    /**
     * @brief Apply the attributes to the document.
     */
    void apply();

private:
    QString                     m_name;
    bool                        m_displayAlone;
    GDocumentViewForGraphics    *m_doc;
    IAttributes                 *m_attributes;

protected:

    virtual bool process(GDocumentViewForGraphics *doc) = 0;
    void setAttributes(const IAttributes *att);
    virtual void attributesDeleted() = 0;

private slots:
    void slotAttributesDeleted();
};

#endif // DM_ABSTRACTATTRIBUTES_H
