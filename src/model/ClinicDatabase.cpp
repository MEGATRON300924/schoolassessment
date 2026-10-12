#include "ClinicDatabase.h"
#include "utils/Algorithms.h"
#include <algorithm>
#include <utility>
namespace Model {
void ClinicDatabase::addPatient(PatientPtr p){if(!p)throw std::invalid_argument("Patient record is empty.");if(findPatient(p->patientID()))throw DuplicateIDException("That patient ID already exists.");patients_.push_back(std::move(p));touchRecent(patients_.back()->patientID());}
ClinicDatabase::PatientPtr ClinicDatabase::removePatient(const std::string& id){for(auto i=patients_.begin();i!=patients_.end();++i)if((*i)->patientID()==id){auto p=*i;patients_.erase(i);undo_.push(AdminCommand{AdminCommand::Type::RestorePatient,p,{},{},{},0});appointments_.erase(std::remove_if(appointments_.begin(),appointments_.end(),[&](const Appointment&a){return a.patientId==id;}),appointments_.end());return p;}return {};}
void ClinicDatabase::editPatient(const std::string&id,const std::string&name,int age,const std::string&contact){auto p=findPatient(id);if(!p)throw std::invalid_argument("Patient not found.");AdminCommand c{AdminCommand::Type::RestoreEdit,p,{},p->name(),p->contact(),p->age()};p->setName(name);p->setAge(age);p->setContact(contact);undo_.push(c);touchRecent(id);}
ClinicDatabase::PatientPtr ClinicDatabase::findPatient(const std::string&id)const{for(const auto&p:patients_)if(p->patientID()==id)return p;return {};}
void ClinicDatabase::enqueue(const std::string&id){auto p=findPatient(id);if(!p)throw std::invalid_argument("Register the patient before adding them to the queue.");waiting_.push(p);}
ClinicDatabase::PatientPtr ClinicDatabase::callNext(){if(waiting_.empty())return {};auto p=waiting_.front();waiting_.pop();return p;}
void ClinicDatabase::scheduleAppointment(const std::string&pid,const std::string&date,const std::string&reason){if(!findPatient(pid))throw std::invalid_argument("Patient not found.");if(date.empty())throw std::invalid_argument("Appointment date/time is required.");Appointment a{"APT-"+std::to_string(nextAppointment_++),pid,date,reason,false};appointments_.push_back(a);}
bool ClinicDatabase::cancelAppointment(const std::string&id){for(auto i=appointments_.begin();i!=appointments_.end();++i)if(i->id==id){undo_.push(AdminCommand{AdminCommand::Type::RestoreAppointment,{},*i,{},{},0});appointments_.erase(i);return true;}return false;}
bool ClinicDatabase::undo(){if(undo_.empty())return false;auto c=undo_.top();undo_.pop();if(c.type==AdminCommand::Type::RestorePatient){if(!findPatient(c.patient->patientID()))patients_.push_back(c.patient);}else if(c.type==AdminCommand::Type::RestoreAppointment){appointments_.push_back(c.appointment);}else if(c.type==AdminCommand::Type::RestoreEdit&&c.patient){c.patient->setName(c.oldName);c.patient->setAge(c.oldAge);c.patient->setContact(c.oldContact);}return true;}
void ClinicDatabase::restorePatient(PatientPtr p){if(p&&!findPatient(p->patientID()))patients_.push_back(p);}
void ClinicDatabase::restoreAppointment(const Appointment&a){appointments_.push_back(a);}
void ClinicDatabase::restoreEdit(const AdminCommand&c){if(c.patient){c.patient->setName(c.oldName);c.patient->setAge(c.oldAge);c.patient->setContact(c.oldContact);}}
void ClinicDatabase::touchRecent(const std::string&id){recent_[recentNext_++%recent_.size()]=id;}
void ClinicDatabase::clear(){patients_.clear();appointments_.clear();waiting_={};undo_={};recent_={};recentNext_=0;}
void ClinicDatabase::loadData(const std::vector<PatientPtr>&p,const std::vector<Appointment>&a){clear();for(const auto&x:p)addPatient(x);appointments_=a;nextAppointment_=static_cast<unsigned long>(appointments_.size()+1);}
void ClinicDatabase::reorderPatients(const std::vector<PatientPtr>& ordered){if(ordered.size()!=patients_.size())throw std::invalid_argument("Sorted patient list does not match database size.");for(const auto& p:ordered)if(!p||!findPatient(p->patientID()))throw std::invalid_argument("Sorted patient list contains an unknown record.");patients_=ordered;}
}
