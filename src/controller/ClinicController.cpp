#include "ClinicController.h"
#include "utils/FileHandler.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <stdexcept>
namespace Controller {
void ClinicController::sort(Utils::SortKey k,bool quick){auto copy=db_.patients();if(quick)Utils::quickSort(copy,k);else Utils::mergeSort(copy,k);auto ap=db_.appointments();db_.clear();for(auto&p:copy)db_.addPatient(p);for(const auto&a:ap)db_.restoreAppointment(a);}
int ClinicController::searchId(const std::string&id)const{return Utils::linearSearchById(db_.patients(),id);}
int ClinicController::searchName(const std::string&name)const{return Utils::recursiveSearchByName(db_.patients(),name);}
void ClinicController::save()const{
 QJsonObject root;QJsonArray patients,appointments;
 for(const auto&p:db_.patients()){QJsonObject o;o["id"]=QString::fromStdString(p->patientID());o["name"]=QString::fromStdString(p->name());o["age"]=p->age();o["contact"]=QString::fromStdString(p->contact());o["category"]=QString::fromStdString(p->category());patients.append(o);}
 for(const auto&a:db_.appointments()){QJsonObject o;o["id"]=QString::fromStdString(a.id);o["patientId"]=QString::fromStdString(a.patientId);o["dateTime"]=QString::fromStdString(a.dateTime);o["reason"]=QString::fromStdString(a.reason);o["completed"]=a.completed;appointments.append(o);}
 root["version"]=1;root["patients"]=patients;root["appointments"]=appointments;
 Utils::FileHandler::write(Utils::FileHandler::defaultPath(),QJsonDocument(root).toJson(QJsonDocument::Indented));
}
void ClinicController::load(){auto bytes=Utils::FileHandler::read(Utils::FileHandler::defaultPath());if(bytes.isEmpty())return;QJsonParseError error;auto doc=QJsonDocument::fromJson(bytes,&error);if(error.error!=QJsonParseError::NoError||!doc.isObject())throw std::runtime_error("Saved JSON is invalid.");std::vector<Model::ClinicDatabase::PatientPtr> ps;std::vector<Model::Appointment> as;auto root=doc.object();
 for(const auto v:root.value("patients").toArray()){auto o=v.toObject();ps.push_back(Model::makePatient(o.value("category").toString("General").toStdString(),o.value("id").toString().toStdString(),o.value("name").toString().toStdString(),o.value("age").toInt(),o.value("contact").toString().toStdString()));}
 for(const auto v:root.value("appointments").toArray()){auto o=v.toObject();as.push_back({o.value("id").toString().toStdString(),o.value("patientId").toString().toStdString(),o.value("dateTime").toString().toStdString(),o.value("reason").toString().toStdString(),o.value("completed").toBool()});}
 db_.loadData(ps,as);
}
}
