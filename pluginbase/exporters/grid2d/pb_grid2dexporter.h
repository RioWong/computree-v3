#ifndef PB_GRID2DEXPORTER_H
#define PB_GRID2DEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_Grid2DExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_Grid2DExporter();
    ~PB_Grid2DExporter();

    virtual QString getExporterCustomName() const;

    void init();

    bool setItemDrawableToExport(const QList<ItemDrawable*> &list);

    bool configureExport();

    virtual IExporter* copy() const;

protected:

    bool protectedExportToFile();
};

#endif // PB_GRID2DEXPORTER_H
