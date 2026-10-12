#pragma once
#include <memory>
#include <stdexcept>
#include <string>

namespace Model {
class InvalidAgeException : public std::invalid_argument { public: using std::invalid_argument::invalid_argument; };
class DuplicateIDException : public std::runtime_error { public: using std::runtime_error::runtime_error; };

class PatientRecord {
public:
    PatientRecord(std::string id, std::string name, int age, std::string contact);
    virtual ~PatientRecord() = default;
    const std::string& patientID() const noexcept { return id_; }
    const std::string& name() const noexcept { return name_; }
    int age() const noexcept { return age_; }
    const std::string& contact() const noexcept { return contact_; }
    void setName(std::string value);
    void setAge(int value);
    void setContact(std::string value);
    virtual std::string category() const = 0;
    virtual int priority() const noexcept = 0;
    virtual std::string careNote() const = 0;
private:
    std::string id_, name_, contact_;
    int age_;
};
class GeneralPatient final : public PatientRecord {
public: using PatientRecord::PatientRecord;
    std::string category() const override { return "General"; }
    int priority() const noexcept override { return 2; }
    std::string careNote() const override { return "Standard consultation"; }
};
class EmergencyPatient final : public PatientRecord {
public: using PatientRecord::PatientRecord;
    std::string category() const override { return "Emergency"; }
    int priority() const noexcept override { return 0; }
    std::string careNote() const override { return "Urgent assessment required"; }
};
class FollowUpPatient final : public PatientRecord {
public: using PatientRecord::PatientRecord;
    std::string category() const override { return "Follow-up"; }
    int priority() const noexcept override { return 1; }
    std::string careNote() const override { return "Review previous consultation"; }
};
std::shared_ptr<PatientRecord> makePatient(const std::string& category, const std::string& id,
                                           const std::string& name, int age, const std::string& contact);
}
