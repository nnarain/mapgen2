#include "output_gen/output_config.h"


OutputConfig::OutputConfig()
{
}

OutputConfig::ExportedFields& OutputConfig::getExportedField()
{
    return exported_fields_;
}