#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <map>
#include <string>
#include <memory>

namespace ui {

template <class Identifier, class Resource>
class ResourceManager {
public:
    ResourceManager() = default;

    void load(const Identifier& id, const std::string& file) {
        auto resource = std::make_unique<Resource>();
        if (!resource->loadFromFile(file)) {
            throw std::runtime_error("ResourceManager::load - Failed to load " + file);
        }
        _resources.insert({id, std::move(resource)});
    }

    const Resource& get(const Identifier& id) const {
        auto pair = _resources.find(id);
        if (pair == _resources.end()) {
            throw std::runtime_error("Resource not found");
        }
        return *(pair->second);
    }

private:
    std::map<Identifier, std::unique_ptr<Resource>> _resources;
};

}

#endif