#pragma once
#include <string>
namespace Model {
class Appointable {
public:
    virtual ~Appointable() = default;
    virtual void scheduleAppointment(const std::string& patientId, const std::string& dateTime, const std::string& reason) = 0;
    virtual bool cancelAppointment(const std::string& appointmentId) = 0;
};
struct Appointment {
    std::string id, patientId, dateTime, reason;
    bool completed = false;
};
}
