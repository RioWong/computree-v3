#include "dm_abstractattributes.h"

#include "interfaces.h"

DM_AbstractAttributes::DM_AbstractAttributes() : DM_AbstractWorker()
{
    m_displayAlone = false;
    m_doc = NULL;
    m_attributes = NULL;
}

DM_AbstractAttributes::~DM_AbstractAttributes()
{
}

void DM_AbstractAttributes::setDisplayableName(const QString &name)
{
    m_name = name;
}

QString DM_AbstractAttributes::displayableName() const
{
    if(m_name.isEmpty()
            && (abstractAttributes() != NULL))
        return abstractAttributes()->getModel()->displayableName();

    return m_name;
}

IAttributes* DM_AbstractAttributes::abstractAttributes() const
{
    return m_attributes;
}

void DM_AbstractAttributes::setDisplayAlone(bool val)
{
    m_displayAlone = val;
}

bool DM_AbstractAttributes::isDisplayedAlone() const
{
    return m_displayAlone;
}

void DM_AbstractAttributes::setDocument(const GDocumentViewForGraphics *doc)
{
    m_doc = (GDocumentViewForGraphics*)doc;
}

GDocumentViewForGraphics *DM_AbstractAttributes::document() const
{
    return m_doc;
}

void DM_AbstractAttributes::apply()
{
    setProgress(0);
    setError("");

    if(!process(document()) && !isAnError())
        setError(tr("Une erreur inconnu est survenu lors du traitement."));

    setFinished();
}

void DM_AbstractAttributes::setAttributes(const IAttributes *att)
{
    m_attributes = (IAttributes*)att;

    if(att != NULL)
        connect(att->getItemDrawableSignalSlotManager(), SIGNAL(destroyed()), this, SLOT(slotAttributesDeleted()), Qt::DirectConnection);
}

void DM_AbstractAttributes::slotAttributesDeleted()
{
    m_attributes = NULL;
    attributesDeleted();
}
