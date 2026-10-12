#pragma once
#include "model/ClinicDatabase.h"
#include "utils/Algorithms.h"
#include <QString>
namespace Controller {
class ClinicController {
public:
    Model::ClinicDatabase& database() noexcept { return db_; }
    const Model::ClinicDatabase& database() const noexcept { return db_; }
    void load();
    void save() const;
    void sort(Utils::SortKey key, bool useQuickSort);
    int searchId(const std::string& id) const;
    int searchName(const std::string& name) const;
private:
    Model::ClinicDatabase db_;
};
}
