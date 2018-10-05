#include "noise_gen/noise_map_manager.h"

NoiseMapManager::NoiseMapManager()
{
}

void NoiseMapManager::createNoiseMap(const std::string& name)
{
    if (noise_maps_.find(name) == noise_maps_.end())
    {
        noise_maps_[name] = std::make_shared<NoiseMap>();
    }
}

void NoiseMapManager::removeNoiseMap(const std::string& name)
{
    if (noise_maps_.find(name) != noise_maps_.end())
    {
        noise_maps_.erase(name);
    }
}

void NoiseMapManager::forEach(std::function<void(const std::string&, NoiseMap&)> fn)
{
    for (auto& pair : noise_maps_)
    {
        fn(pair.first, *pair.second);
    }
}

NoiseMap::Ptr& NoiseMapManager::getNoiseMap(const std::string& name)
{
    return noise_maps_[name];
}

NoiseMap::Ptr& NoiseMapManager::operator[](const std::string name)
{
    return getNoiseMap(name);
}