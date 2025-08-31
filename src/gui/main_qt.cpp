#include <QApplication>
#include <QFont>
#include "MainWindow.hpp"

int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    QFont f = app.font(); f.setPointSize(10); app.setFont(f);
    MainWindow w; w.show();
    return app.exec();
}

