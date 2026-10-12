#pragma once
#include "Appointment.h"
#include "PatientRecord.h"
#include <array>
#include <memory>
#include <queue>
#include <stack>
#include <vector>
#include <string>
namespace Model {
struct AdminCommand { enum class Type { RestorePatient, RestoreAppointment, RestoreEdit }; Type type; std::shared_ptr<PatientRecord> patient; Appointment appointment; std::string oldName, oldContact; int oldAge=0; };
class ClinicDatabase : public Appointable {
public:
    using PatientPtr=std::shared_ptr<PatientRecord>;
    const std::vector<PatientPtr>& patients() const noexcept { return patients_; }
    const std::vector<Appointment>& appointments() const noexcept { return appointments_; }
    const std::queue<PatientPtr>& waitingQueue() const noexcept { return waiting_; }
    void addPatient(PatientPtr);
    PatientPtr removePatient(const std::string&);
    void editPatient(const std::string&, const std::string&, int, const std::string&);
    PatientPtr findPatient(const std::string&) const;
    void enqueue(const std::string&);
    PatientPtr callNext();
    void scheduleAppointment(const std::string&, const std::string&, const std::string&) override;
    bool cancelAppointment(const std::string&) override;
    bool undo();
    void clear();
    void restorePatient(PatientPtr);
    void restoreAppointment(const Appointment&);
    void restoreEdit(const AdminCommand&);
    const std::array<std::string,10>& recentCache() const noexcept { return recent_; }
    void touchRecent(const std::string&);
    std::stack<AdminCommand>& undoStack() noexcept { return undo_; }
    void loadData(const std::vector<PatientPtr>&, const std::vector<Appointment>&);
    void reorderPatients(const std::vector<PatientPtr>& ordered);
private:
    std::vector<PatientPtr> patients_;
    std::vector<Appointment> appointments_;
    std::queue<PatientPtr> waiting_;
    std::stack<AdminCommand> undo_;
    std::array<std::string,10> recent_{};
    std::size_t recentNext_=0;
    unsigned long nextAppointment_=1;
};
}
