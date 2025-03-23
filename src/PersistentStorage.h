#include <string>

class PersistentStorage {
public:
    PersistentStorage(const std::string& path);
    ~PersistentStorage();
};