#ifndef PB_PGMEXPORTER_H
#define PB_PGMEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_PgmExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_PgmExporter();
    ~PB_PgmExporter();

    virtual QString getExporterCustomName() const;

    void init();

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);

    bool configureExport();

    virtual CT_AbstractExporter* copy() const;

protected:
    bool protectedExportToFile();
};

#endif // PB_PGMEXPORTER_H
