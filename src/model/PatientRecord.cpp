#include "PatientRecord.h"
#include <utility>
namespace Model {
PatientRecord::PatientRecord(std::string id, std::string name, int age, std::string contact)
    : id_(std::move(id)), name_(std::move(name)), contact_(std::move(contact)), age_(age) {
    if (id_.empty()) throw std::invalid_argument("Patient ID cannot be empty.");
    if (name_.empty()) throw std::invalid_argument("Patient name cannot be empty.");
    setAge(age_);
}
void PatientRecord::setName(std::string value) {
    if (value.empty()) throw std::invalid_argument("Patient name cannot be empty.");
    name_ = std::move(value);
}
void PatientRecord::setAge(int value) {
    if (value < 0 || value > 120) throw InvalidAgeException("Age must be between 0 and 120.");
    age_ = value;
}
void PatientRecord::setContact(std::string value) { contact_ = std::move(value); }
std::shared_ptr<PatientRecord> makePatient(const std::string& category, const std::string& id,
                                           const std::string& name, int age, const std::string& contact) {
    if (category == "Emergency") return std::make_shared<EmergencyPatient>(id, name, age, contact);
    if (category == "Follow-up") return std::make_shared<FollowUpPatient>(id, name, age, contact);
    return std::make_shared<GeneralPatient>(id, name, age, contact);
}
}
