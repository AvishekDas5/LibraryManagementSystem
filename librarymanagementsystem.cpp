#include "librarymanagementsystem.h"
#include "./ui_librarymanagementsystem.h"

LibraryManagementSystem::LibraryManagementSystem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LibraryManagementSystem)
{
    ui->setupUi(this);
}

LibraryManagementSystem::~LibraryManagementSystem()
{
    delete ui;
}
