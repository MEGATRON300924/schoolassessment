#pragma once
#include "controller/ClinicController.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QTimer>
class QLineEdit; class QComboBox; class QSpinBox; class QTabWidget; class QLabel;
namespace UI {
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(Controller::ClinicController& controller, QWidget* parent=nullptr);
private:
    Controller::ClinicController& controller_;
    QTableWidget *patientsTable_, *appointmentsTable_, *queueTable_;
    QLineEdit *idEdit_, *nameEdit_, *contactEdit_, *searchEdit_, *reasonEdit_;
    QSpinBox *ageEdit_;
    QComboBox *categoryBox_, *sortKeyBox_, *algorithmBox_;
    QLabel *status_;
    QTimer timer_;
    void buildUi();
    void refresh();
    void addPatient();
    void editPatient();
    void deletePatient();
    void scheduleAppointment();
    void cancelAppointment();
    void addToQueue();
    void callNext();
    void search();
    void sort();
    void showReports();
    void saveNow();
};
}
