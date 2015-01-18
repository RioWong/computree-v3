#ifndef CT_READER_ASCRGB_H
#define CT_READER_ASCRGB_H

#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_pointsattributescolor.h"

#include "ct_reader/ct_reader_ascrgb_def_models.h"

class PLUGINSHAREDSHARED_EXPORT CT_Reader_ASCRGB : public CT_AbstractReader
{
    Q_OBJECT

public:
    CT_Reader_ASCRGB();

    bool setFilePath(const QString &filepath);
    void setRadiusFilter(const double &radius);

    CT_AbstractReader* copy() const;

private:
    double   m_filterRadius;

protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();
};

#endif // CT_READER_ASCRGB_H
