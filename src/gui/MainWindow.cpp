#include "MainWindow.h"
#include "utils/FileHandler.h"
#include <QAbstractItemView>
#include <QAction>
#include <QColor>
#include <QComboBox>
#include <QDate>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QFormLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QKeySequence>
#include <QMenuBar>
namespace UI {
MainWindow::MainWindow(Controller::ClinicController& c,QWidget* p):QMainWindow(p),controller_(c){
 setWindowTitle("SCAPMS — Smart Clinic Management");resize(1100,720);buildUi();
 connect(&timer_,&QTimer::timeout,this,[this]{refresh();});timer_.start(15000);refresh();
}
void MainWindow::buildUi(){
 auto* root=new QWidget(this);auto* layout=new QVBoxLayout(root);auto* tabs=new QTabWidget(root);
 auto* records=new QWidget;auto* rl=new QVBoxLayout(records);auto* form=new QFormLayout;
 idEdit_=new QLineEdit;idEdit_->setPlaceholderText("Unique patient ID");nameEdit_=new QLineEdit;contactEdit_=new QLineEdit;ageEdit_=new QSpinBox;ageEdit_->setRange(0,120);
 categoryBox_=new QComboBox;categoryBox_->addItems({"General","Emergency","Follow-up"});
 form->addRow("Patient ID",idEdit_);form->addRow("Full name",nameEdit_);form->addRow("Age",ageEdit_);form->addRow("Contact",contactEdit_);form->addRow("Category",categoryBox_);rl->addLayout(form);
 auto* actions=new QHBoxLayout;auto* add=new QPushButton("Add patient");auto* edit=new QPushButton("Edit selected");auto* del=new QPushButton("Delete selected");auto* undo=new QPushButton("Undo last operation");actions->addWidget(add);actions->addWidget(edit);actions->addWidget(del);actions->addWidget(undo);rl->addLayout(actions);
 patientsTable_=new QTableWidget(0,5);patientsTable_->setHorizontalHeaderLabels({"ID","Name","Age","Contact","Category"});patientsTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);patientsTable_->setSelectionBehavior(QAbstractItemView::SelectRows);patientsTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);rl->addWidget(patientsTable_);tabs->addTab(records,"Patient Records");
 auto* appt=new QWidget;auto* al=new QVBoxLayout(appt);auto* af=new QFormLayout;auto* patientId=new QLineEdit;patientId->setPlaceholderText("Registered patient ID");auto* date=new QDateTimeEdit(QDateTime::currentDateTime().addDays(1));date->setCalendarPopup(true);date->setDisplayFormat("yyyy-MM-dd HH:mm");reasonEdit_=new QLineEdit;af->addRow("Patient ID",patientId);af->addRow("Date and time",date);af->addRow("Reason",reasonEdit_);al->addLayout(af);auto* schedule=new QPushButton("Schedule appointment");auto* cancel=new QPushButton("Cancel selected appointment");al->addWidget(schedule);al->addWidget(cancel);appointmentsTable_=new QTableWidget(0,5);appointmentsTable_->setHorizontalHeaderLabels({"Appointment ID","Patient ID","Date/time","Reason","Status"});appointmentsTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);appointmentsTable_->setSelectionBehavior(QAbstractItemView::SelectRows);appointmentsTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);al->addWidget(appointmentsTable_);tabs->addTab(appt,"Appointments");
 connect(schedule,&QPushButton::clicked,this,[this,patientId,date]{try{controller_.database().scheduleAppointment(patientId->text().trimmed().toStdString(),date->dateTime().toString(Qt::ISODate).toStdString(),reasonEdit_->text().trimmed().toStdString());refresh();}catch(const std::exception&e){QMessageBox::warning(this,"Cannot schedule",e.what());}});
 connect(cancel,&QPushButton::clicked,this,&MainWindow::cancelAppointment);
 auto* queue=new QWidget;auto* ql=new QVBoxLayout(queue);auto* qrow=new QHBoxLayout;auto* qid=new QLineEdit;qid->setPlaceholderText("Patient ID");auto* enqueue=new QPushButton("Add to waiting queue");auto* next=new QPushButton("Call next patient");qrow->addWidget(qid);qrow->addWidget(enqueue);qrow->addWidget(next);ql->addLayout(qrow);queueTable_=new QTableWidget(0,3);queueTable_->setHorizontalHeaderLabels({"Queue position","Patient ID","Name"});queueTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);ql->addWidget(queueTable_);tabs->addTab(queue,"Waiting Queue");
 connect(enqueue,&QPushButton::clicked,this,[this,qid]{try{controller_.database().enqueue(qid->text().trimmed().toStdString());refresh();}catch(const std::exception&e){QMessageBox::warning(this,"Queue",e.what());}});connect(next,&QPushButton::clicked,this,&MainWindow::callNext);
 auto* tools=new QWidget;auto* tl=new QVBoxLayout(tools);auto* sf=new QHBoxLayout;searchEdit_=new QLineEdit;searchEdit_->setPlaceholderText("Search by patient ID or name");auto* sb=new QPushButton("Search");sortKeyBox_=new QComboBox;sortKeyBox_->addItems({"Name","ID","Age"});algorithmBox_=new QComboBox;algorithmBox_->addItems({"Quick sort","Merge sort"});auto* sortb=new QPushButton("Sort records");sf->addWidget(searchEdit_);sf->addWidget(sb);sf->addWidget(sortKeyBox_);sf->addWidget(algorithmBox_);sf->addWidget(sortb);tl->addLayout(sf);tl->addWidget(new QLabel("Search uses custom linear ID search or recursive name search. Binary search is available in the algorithms module for ID-sorted records."));tabs->addTab(tools,"Search & Sort");connect(sb,&QPushButton::clicked,this,&MainWindow::search);connect(sortb,&QPushButton::clicked,this,&MainWindow::sort);
 auto* reports=new QWidget;auto* reportLayout=new QVBoxLayout(reports);auto* reportButton=new QPushButton("Generate clinic summary");status_=new QLabel("Ready");status_->setWordWrap(true);reportLayout->addWidget(reportButton);reportLayout->addWidget(status_);tabs->addTab(reports,"Reports");connect(reportButton,&QPushButton::clicked,this,&MainWindow::showReports);
 layout->addWidget(tabs);setCentralWidget(root);
 connect(add,&QPushButton::clicked,this,&MainWindow::addPatient);connect(edit,&QPushButton::clicked,this,&MainWindow::editPatient);connect(del,&QPushButton::clicked,this,&MainWindow::deletePatient);connect(undo,&QPushButton::clicked,this,[this]{if(!controller_.database().undo())QMessageBox::information(this,"Undo","There is no administrative operation to undo.");refresh();});
 auto* saveAct=new QAction("Save",this);saveAct->setShortcut(QKeySequence::Save);connect(saveAct,&QAction::triggered,this,&MainWindow::saveNow);addAction(saveAct);
 auto* findAct=new QAction("Find",this);findAct->setShortcut(QKeySequence::Find);connect(findAct,&QAction::triggered,this,[this]{searchEdit_->setFocus();search();});addAction(findAct);
 auto* quitAct=new QAction("Quit",this);quitAct->setShortcut(QKeySequence::Quit);connect(quitAct,&QAction::triggered,this,&QWidget::close);addAction(quitAct);
}
void MainWindow::refresh(){
 const auto& db=controller_.database();patientsTable_->setRowCount(0);
 for(const auto&p:db.patients()){int r=patientsTable_->rowCount();patientsTable_->insertRow(r);QStringList vals{QString::fromStdString(p->patientID()),QString::fromStdString(p->name()),QString::number(p->age()),QString::fromStdString(p->contact()),QString::fromStdString(p->category())};for(int c=0;c<vals.size();++c){auto* item=new QTableWidgetItem(vals[c]);if(p->category()=="Emergency"){item->setBackground(QColor(255,220,220));item->setForeground(QColor(140,0,0));}patientsTable_->setItem(r,c,item);}}
 appointmentsTable_->setRowCount(0);for(const auto&a:db.appointments()){int r=appointmentsTable_->rowCount();appointmentsTable_->insertRow(r);QStringList vals{QString::fromStdString(a.id),QString::fromStdString(a.patientId),QString::fromStdString(a.dateTime),QString::fromStdString(a.reason),a.completed?"Completed":"Pending"};for(int c=0;c<vals.size();++c)appointmentsTable_->setItem(r,c,new QTableWidgetItem(vals[c]));}
 auto queue=db.waitingQueue();queueTable_->setRowCount(0);int pos=1;while(!queue.empty()){auto p=queue.front();queue.pop();int r=queueTable_->rowCount();queueTable_->insertRow(r);queueTable_->setItem(r,0,new QTableWidgetItem(QString::number(pos++)));queueTable_->setItem(r,1,new QTableWidgetItem(QString::fromStdString(p->patientID())));queueTable_->setItem(r,2,new QTableWidgetItem(QString::fromStdString(p->name())));}
}
void MainWindow::addPatient(){try{auto p=Model::makePatient(categoryBox_->currentText().toStdString(),idEdit_->text().trimmed().toStdString(),nameEdit_->text().trimmed().toStdString(),ageEdit_->value(),contactEdit_->text().trimmed().toStdString());controller_.database().addPatient(p);idEdit_->clear();nameEdit_->clear();contactEdit_->clear();refresh();}catch(const std::exception&e){QMessageBox::warning(this,"Invalid patient",e.what());}}
void MainWindow::editPatient(){int r=patientsTable_->currentRow();if(r<0){QMessageBox::information(this,"Edit","Select a patient row first.");return;}auto id=patientsTable_->item(r,0)->text().toStdString();auto p=controller_.database().findPatient(id);if(!p)return;bool ok=false;QString name=QInputDialog::getText(this,"Edit patient","Name:",QLineEdit::Normal,QString::fromStdString(p->name()),&ok);if(!ok)return;int age=QInputDialog::getInt(this,"Edit patient","Age:",p->age(),0,120,1,&ok);if(!ok)return;QString contact=QInputDialog::getText(this,"Edit patient","Contact:",QLineEdit::Normal,QString::fromStdString(p->contact()),&ok);if(!ok)return;try{controller_.database().editPatient(id,name.trimmed().toStdString(),age,contact.trimmed().toStdString());refresh();}catch(const std::exception&e){QMessageBox::warning(this,"Edit failed",e.what());}}
void MainWindow::deletePatient(){int r=patientsTable_->currentRow();if(r<0){QMessageBox::information(this,"Delete","Select a patient row first.");return;}auto id=patientsTable_->item(r,0)->text().toStdString();if(QMessageBox::question(this,"Confirm delete","Delete selected patient?")!=QMessageBox::Yes)return;if(!controller_.database().removePatient(id))QMessageBox::warning(this,"Delete","Patient not found.");refresh();}
void MainWindow::scheduleAppointment(){}
void MainWindow::cancelAppointment(){int r=appointmentsTable_->currentRow();if(r<0){QMessageBox::information(this,"Cancel","Select an appointment first.");return;}auto id=appointmentsTable_->item(r,0)->text().toStdString();if(!controller_.database().cancelAppointment(id))QMessageBox::warning(this,"Cancel","Appointment not found.");refresh();}
void MainWindow::addToQueue(){}
void MainWindow::callNext(){auto p=controller_.database().callNext();if(!p)QMessageBox::information(this,"Waiting queue","The waiting queue is empty.");else QMessageBox::information(this,"Next patient",QString("Please call %1 (%2).").arg(QString::fromStdString(p->name()),QString::fromStdString(p->patientID())));refresh();}
void MainWindow::search(){auto q=searchEdit_->text().trimmed().toStdString();if(q.empty())return;int i=controller_.searchId(q);if(i<0)i=controller_.searchName(q);if(i<0)QMessageBox::information(this,"Search","No matching patient found.");else{patientsTable_->selectRow(i);patientsTable_->scrollToItem(patientsTable_->item(i,0));}}
void MainWindow::sort(){Utils::SortKey k=sortKeyBox_->currentText()=="ID"?Utils::SortKey::ID:(sortKeyBox_->currentText()=="Age"?Utils::SortKey::Age:Utils::SortKey::Name);controller_.sort(k,algorithmBox_->currentIndex()==0);refresh();}
void MainWindow::showReports(){int general=0,emergency=0,follow=0;for(const auto&p:controller_.database().patients()){if(p->category()=="Emergency")++emergency;else if(p->category()=="Follow-up")++follow;else ++general;}int pending=0;for(const auto&a:controller_.database().appointments())if(!a.completed){++pending;}QString today=QDate::currentDate().toString(Qt::ISODate);status_->setText(QString("Patients: %1\nGeneral: %2 | Emergency: %3 | Follow-up: %4\nPending appointments: %5\nToday's date: %6").arg(controller_.database().patients().size()).arg(general).arg(emergency).arg(follow).arg(pending).arg(today));}
void MainWindow::saveNow(){try{controller_.save();status_->setText("Data saved to "+Utils::FileHandler::defaultPath());}catch(const std::exception&e){QMessageBox::critical(this,"Save failed",e.what());}}
}
