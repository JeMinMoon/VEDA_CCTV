#include "mainwindow.h"
#include <QApplication>
#include <gst/gst.h>
#include <windows.h>
#include <string>
#include <codecvt> // for std::wstring_convert
#include <locale>   // for std::locale

int main(int argc, char *argv[])
{
    // std::string gstPluginPath = QApplication::applicationDirPath().toStdString() + "/gstreamer/lib/gstreamer-1.0";
    // // Convert std::string to std::wstring
    // std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    // std::wstring wideGstPluginPath = converter.from_bytes(gstPluginPath);

    // // Set the environment variable
    // SetEnvironmentVariable(wideGstPluginPath.c_str(), NULL); // Set value to NULL to delete it

    gst_init (&argc, &argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
