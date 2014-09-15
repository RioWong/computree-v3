#include "ct_abstractcategory.h"

#include "math.h"
#include "ct_global/ct_context.h"
#include "ct_categories/tools/ct_categorymanager.h"

#include <QMetaEnum>

const QString CT_AbstractCategory::DATA_VALUE                   = CT_AbstractCategory::staticInitDataValue();
const QString CT_AbstractCategory::DATA_ID                      = CT_AbstractCategory::staticInitDataId();
const QString CT_AbstractCategory::DATA_DISPLAYABLE_NAME        = CT_AbstractCategory::staticInitDataDisplayableName();
const QString CT_AbstractCategory::DATA_COORDINATE              = CT_AbstractCategory::staticInitDataCoordinate();
const QString CT_AbstractCategory::DATA_X                       = CT_AbstractCategory::staticInitDataX();
const QString CT_AbstractCategory::DATA_Y                       = CT_AbstractCategory::staticInitDataY();
const QString CT_AbstractCategory::DATA_Z                       = CT_AbstractCategory::staticInitDataZ();
const QString CT_AbstractCategory::DATA_CX                      = CT_AbstractCategory::staticInitDataCx();
const QString CT_AbstractCategory::DATA_CY                      = CT_AbstractCategory::staticInitDataCy();
const QString CT_AbstractCategory::DATA_CZ                      = CT_AbstractCategory::staticInitDataCz();
const QString CT_AbstractCategory::DATA_DIRECTION               = CT_AbstractCategory::staticInitDataDirection();
const QString CT_AbstractCategory::DATA_X_DIRECTION             = CT_AbstractCategory::staticInitDataXDirection();
const QString CT_AbstractCategory::DATA_Y_DIRECTION             = CT_AbstractCategory::staticInitDataYDirection();
const QString CT_AbstractCategory::DATA_Z_DIRECTION             = CT_AbstractCategory::staticInitDataZDirection();
const QString CT_AbstractCategory::DATA_DIMENSION               = CT_AbstractCategory::staticInitDataDimension();
const QString CT_AbstractCategory::DATA_X_DIMENSION             = CT_AbstractCategory::staticInitDataXDimension();
const QString CT_AbstractCategory::DATA_Y_DIMENSION             = CT_AbstractCategory::staticInitDataYDimension();
const QString CT_AbstractCategory::DATA_Z_DIMENSION             = CT_AbstractCategory::staticInitDataZDimension();
const QString CT_AbstractCategory::DATA_RADIUS                  = CT_AbstractCategory::staticInitDataRadius();
const QString CT_AbstractCategory::DATA_R2                      = CT_AbstractCategory::staticInitDataR2();
const QString CT_AbstractCategory::DATA_SIZE                    = CT_AbstractCategory::staticInitDataSize();
const QString CT_AbstractCategory::DATA_NUMBER                  = CT_AbstractCategory::staticInitDataNumber();
const QString CT_AbstractCategory::DATA_LENGTH                  = CT_AbstractCategory::staticInitDataLength();
const QString CT_AbstractCategory::DATA_WIDTH                   = CT_AbstractCategory::staticInitDataWidth();
const QString CT_AbstractCategory::DATA_HEIGHT                  = CT_AbstractCategory::staticInitDataHeight();
const QString CT_AbstractCategory::DATA_ANGLE                   = CT_AbstractCategory::staticInitDataAngle();
const QString CT_AbstractCategory::DATA_RATIO                   = CT_AbstractCategory::staticInitDataRatio();
const QString CT_AbstractCategory::DATA_RESOLUTION              = CT_AbstractCategory::staticInitDataResolution();
const QString CT_AbstractCategory::DATA_X_RESOLUTION            = CT_AbstractCategory::staticInitDataXResolution();
const QString CT_AbstractCategory::DATA_Y_RESOLUTION            = CT_AbstractCategory::staticInitDataYResolution();
const QString CT_AbstractCategory::DATA_Z_RESOLUTION            = CT_AbstractCategory::staticInitDataZResolution();
const QString CT_AbstractCategory::DATA_NA                      = CT_AbstractCategory::staticInitDataNa();

CT_AbstractCategory::CT_AbstractCategory(const QString &uniqueName,
                                         const QString &categoryEquivalentUniqueName,
                                         const QString &displayableName,
                                         const QString &description)
{
    m_uName = uniqueName;
    m_dName = displayableName;
    m_description = description;

    if(m_dName.isEmpty())
        m_dName = m_uName;

    if(!categoryEquivalentUniqueName.isEmpty())
        m_eCategories.append(categoryEquivalentUniqueName);

    internalTestAttributes("CT_AbstractCategory constructor");
}

CT_AbstractCategory::CT_AbstractCategory(const QString &uniqueName,
                                         const QList<QString> &categoryEquivalentUniqueNameCollection,
                                         const QString &displayableName,
                                         const QString &description)
{
    m_uName = uniqueName;
    m_dName = displayableName;
    m_description = description;

    if(m_dName.isEmpty())
        m_dName = m_uName;

    m_eCategories = categoryEquivalentUniqueNameCollection;

    internalTestAttributes("CT_AbstractCategory constructor");
}

bool CT_AbstractCategory::addEquivalentCategory(const QString &categoryEquivalentUniqueName)
{
    if(!m_eCategories.contains(categoryEquivalentUniqueName))
        m_eCategories.append(categoryEquivalentUniqueName);

    return true;
}

QString CT_AbstractCategory::uniqueName() const
{
    return m_uName;
}

QString CT_AbstractCategory::valueTypeToString(CT_AbstractCategory::ValueType v) const
{
    int index = metaObject()->indexOfEnumerator("ValueType");
    QMetaEnum metaEnum = metaObject()->enumerator(index);
    return metaEnum.valueToKey(v);
}

QString CT_AbstractCategory::displayableName() const
{
    return m_dName;
}

QString CT_AbstractCategory::description() const
{
    return m_description;
}

bool CT_AbstractCategory::isEquivalentTo(const CT_AbstractCategory *c) const
{
    if(m_eCategories.contains(c->uniqueName()))
        return true;

    foreach (const QString &v, m_eCategories) {
        if(PS_CATEGORY_MANAGER->findByUniqueName(v)->isEquivalentTo(c))
            return true;
    }

    return false;
}

bool CT_AbstractCategory::isEquivalentTo(const QString &categoryUniqueName) const
{
    if(m_eCategories.contains(categoryUniqueName))
        return true;

    foreach (const QString &v, m_eCategories) {
        if(PS_CATEGORY_MANAGER->findByUniqueName(v)->isEquivalentTo(categoryUniqueName))
            return true;
    }

    return false;
}

void CT_AbstractCategory::internalTestAttributes(const QString &where)
{
    Q_ASSERT_X(!m_uName.isEmpty(), qPrintable(where), "uniqueName is empty");
}

