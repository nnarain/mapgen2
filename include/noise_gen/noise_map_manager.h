#ifndef NOISE_GEN_NOISE_MAP_MANAGER_H
#define NOISE_GEN_NOISE_MAP_MANAGER_H

#include "noise_gen/noise_map.h"

#include <string>
#include <vector>
#include <functional>

class NoiseMapManager
{
public:
    NoiseMapManager();

    void createNoiseMap(const std::string& name);
    void removeNoiseMap(const std::string& name);

    void forEach(std::function<void(const std::string&, NoiseMap&)> fn);

    NoiseMap& getNoiseMap(const std::string& name);
private:
    std::map<std::string, NoiseMap> noise_maps_;
};

#endif //