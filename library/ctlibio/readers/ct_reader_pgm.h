#ifndef CT_READER_PGM_H
#define CT_READER_PGM_H

#include "ct_reader/abstract/ct_abstractreader.h"

#include "ctlibio/ctlibio_global.h"
#include "ct_reader_pgm_def_models.h"

/**
 * @brief Reader that can load a pgm file (*.pgm) that represent a raster 2D
 */
class CTLIBIO_EXPORT CT_Reader_PGM : public CT_AbstractReader
{
    Q_OBJECT

public:

    enum FileType {
        ASCII,
        BINARY
    };

    CT_Reader_PGM();

    /**
     * @brief Returns a displayable name of the reader
     */
    QString GetReaderName() const;

    /**
     * @brief Returns the sub menu level where we can store this reader
     */
    CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    /**
     * @brief Set the filepath to the reader. The file will be opened to search faces, if no face was found
     *        the reader will load a PointCloud otherwise it will load a Mesh.
     */
    bool setFilePath(const QString &filepath);

    CT_AbstractReader* copy() const;

private:

protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();

    bool readHeader(QTextStream &stream, FileType &type, size_t &width, size_t &height, quint16 &maxGray, qint64 *outOffset = NULL) const;
    QString nextLineWithNoComment(QTextStream &stream) const;
};

#endif // CT_READER_PGM_H
