#pragma once

#include "controller/ClinicController.h"

#include <QMainWindow>
#include <QTableWidget>
#include <QTimer>

class QComboBox;
class QLabel;
class QLineEdit;
class QSpinBox;

namespace UI {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(Controller::ClinicController& controller,
                        QWidget* parent = nullptr);

private:
    Controller::ClinicController& controller_;

    QTableWidget* patientsTable_;
    QTableWidget* appointmentsTable_;
    QTableWidget* queueTable_;

    QLineEdit* idEdit_;
    QLineEdit* nameEdit_;
    QLineEdit* contactEdit_;
    QLineEdit* searchEdit_;
    QLineEdit* reasonEdit_;

    QSpinBox* ageEdit_;
    QComboBox* categoryBox_;
    QComboBox* sortKeyBox_;
    QComboBox* algorithmBox_;
    QLabel* status_;

    QTimer refreshTimer_;

    void buildUi();
    void refresh();

    void addPatient();
    void editPatient();
    void deletePatient();
    void cancelAppointment();
    void callNext();

    void search();
    void sort();
    void showReports();
    void saveNow();
};

} // namespace UI
