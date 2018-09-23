#include "ui/view_controller/noise_map_controller.h"

static const std::string empty("");

NoiseMapController::NoiseMapController(NoiseMap& manager)
    : manager_{manager}
    , names_{}
    , is_names_cached_{false}
{

}

NoiseModule::Ptr& NoiseMapController::get(const std::string& name)
{
    return manager_.get(name);
}

bool NoiseMapController::has(const std::string& name)
{
    return manager_.has(name);
}

void NoiseMapController::createModule(const std::string& name, NoiseModule::Type type)
{
    manager_.add(name, type);
    is_names_cached_ = false;
}

void NoiseMapController::removeModule(const std::string& name)
{
    manager_.remove(name);
    is_names_cached_ = false;
}

NoiseModule::Ptr& NoiseMapController::createModuleWithUniqueName(NoiseModule::Type type)
{
    std::size_t i = 0;
    std::string name;

    do
    {
        name = std::string("noise") + std::to_string(i);
        i++;
    }
    while (has(name));

    createModule(name, type);

    return get(name);
}

void NoiseMapController::renameModule(const std::string current_name, const std::string new_name)
{
    manager_.rename(current_name, new_name);
}

void NoiseMapController::setOutputModule(const NoiseModule::Ref output_ref)
{
    output_ref_ = output_ref;
    output_changed_(output_ref_);
}

void NoiseMapController::addOutputChangedObserver(std::function<OutputChangedSignature> fn)
{
    output_changed_.connect(fn);
}

void NoiseMapController::setSeed(int seed)
{
    manager_.setSeed(seed);
}

void NoiseMapController::forEach(std::function<void(const std::string&, NoiseModule&)> fn)
{
    manager_.forEach(fn);
}

const std::vector<std::string>& NoiseMapController::getModuleNames()
{
    if (is_names_cached_)
    {
        // names are cached, return cache names
        return names_;
    }
    else
    {
        names_.clear();

        manager_.forEach([this](const std::string& name, NoiseModule& module)
        {
            names_.push_back(name);
        });

        is_names_cached_ = true;

        return names_;
    }
}

const std::size_t NoiseMapController::size() const noexcept
{
    return manager_.size();
}