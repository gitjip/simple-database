#include "Entity.h"
#include "Exception.h"

Entity::Entity(const std::filesystem::path& path) :
	storagePath(path)
{
	if (path.empty()) {
		throw Exception("File path empty");
	}
}

std::filesystem::path Entity::ToPath(const std::string& stem, const std::string& extension) const
{
	return storagePath.string() + "/" + stem + (extension.empty() ? "" : ".") + extension;
}