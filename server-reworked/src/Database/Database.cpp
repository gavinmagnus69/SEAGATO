#include "Database.h"


Database::Database(const std::shared_ptr<IConfig>& cfg) {
    this->cfg = cfg;
}
