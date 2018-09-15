#ifndef OUTPUT_CONFIG_H
#define OUTPUT_CONFIG_H

#include <map>
#include <string>

class OutputConfig
{
public:
    using ExportedFields = std::map<std::string, bool>;

    OutputConfig();

    ExportedFields& getExportedField();

private:
    ExportedFields exported_fields_;
};

#endif
