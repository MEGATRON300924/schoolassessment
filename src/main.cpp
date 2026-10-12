#include "gui/MainWindow.h"
#include "controller/ClinicController.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("SCAPMS");
    QApplication::setOrganizationName("SCAPMS");
    Controller::ClinicController controller;
    try { controller.load(); }
    catch (const std::exception &e) {
        QMessageBox::warning(nullptr, "Data loading", QString("Some saved data could not be loaded: %1").arg(e.what()));
    }
    UI::MainWindow window(controller);
    window.show();
    const int result = app.exec();
    try { controller.save(); }
    catch (const std::exception &e) {
        QMessageBox::critical(nullptr, "Save failed", QString("Could not save clinic data: %1").arg(e.what()));
        return 1;
    }
    return result;
}
