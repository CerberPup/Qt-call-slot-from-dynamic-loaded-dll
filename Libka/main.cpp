#include "main.h"

#include <iostream>

#include <QtWidgets/QApplication>
#include <QtWidgets/QtWidgets>

void list_widgets()
{
    std::cout << "Lista:" << std::endl;

    const QWidgetList allWidgets = QApplication::allWidgets();
    for (QWidget *widget : allWidgets)
    {
        std::string name = widget->objectName().toStdString();
        std::cout << name << std::endl;
    }
}

void call_slot(std::string _name, std::string _slot)
{
    const QWidgetList allWidgets = QApplication::allWidgets();
    for (QWidget *widget : allWidgets)
    {
        std::string name = widget->objectName().toStdString();
        if (_name == name)
        {
            QMetaObject::invokeMethod( widget, _slot.c_str()/*, Q_ARG( int, param ) */);
        }
    }
}