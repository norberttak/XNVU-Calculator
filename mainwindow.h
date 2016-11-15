#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QtPrintSupport/QtPrintSupport>
#include <NVU.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void quit();
    void showFlightplanContextMenu(const QPoint& pos);
    void showXPlaneSettings();
    void importFMS();
    void exportFMS();
    void loadNVUFlightplan();
    void saveNVUFlightPlan();
    void tableGoUp();
    void tableGoDown();

    //void clearFlightplan();
    //void refreshFlightplan();
    //void insertRoute(std::vector<NVUPOINT*> route, int row, int offset);
    //void updateDistanceAndN();
    //void refreshRow(int row, NVUPOINT* waypoint = NULL);
    //void insertWaypoint(NVUPOINT* wp, int row, int offset);
    //void insertTableWidgetWaypoint(NVUPOINT* waypoint, int row);
    //void replaceTableWidgetWaypoint(NVUPOINT* nvupoint, int row);
    //void deleteWaypoint(int row);
    void deleteCurrentWaypoint();
    void setWaypointDescription(NVUPOINT* wp);

    void printPreview(QPrinter*);
    void drawNVUHeader(QPainter& painter, NVUPOINT *dep, NVUPOINT *arr, double fork, int& y);
    void painterDrawNVUPoint(QPainter& painter, NVUPOINT*wp, int wpNumber, bool isArr, int &y);
    void clearFlightplanTimeout();


    void on_lineEdit_textChanged(const QString &arg1);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButtonPrint_clicked();

    void on_pushButtonInsertBefore_clicked();

    void on_pushButtonReplace_clicked();

    void on_pushButtonInsertAfter_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

    void on_listWidget_currentRowChanged(int currentRow);

    void on_tableWidget_itemSelectionChanged();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_pushButton_ClearFlightplan_clicked();

    void on_pushButtonRouteInsertAfter_clicked();

    void on_pushButtonRouteInsertBefore_clicked();

    void on_pushButtonRouteReplace_clicked();

    void on_actionOptions_triggered();

    void on_frameDescription_clicked();

    //void on_actionXNVU_library_triggered();

    void on_pushButton_showAIRAC_Airports_clicked();

    void on_pushButton_showAIRAC_Navaids_clicked();

    void on_pushButton_showAIRAC_Fixes_clicked();

    void on_pushButton_showAIRAC_Airways_clicked();

    void on_pushButton_showEarthNav_clicked();

    void on_pushButton_showRSBN_clicked();

    void on_pushButton_showXNVU_clicked();

    void on_pushButton_showNDB_clicked();

    void on_pushButton_showVOR_clicked();

    void on_pushButton_showVORDME_clicked();

    void on_pushButton_showDME_clicked();

    void on_pushButton_showFix_clicked();

    void on_pushButton_showAirports_clicked();

    void on_pushButtonSetFL_clicked();

    void on_actionShow_feet_triggered();

    void on_pushButtonDeleteWaypoint_clicked();

    void on_actionColumns_2_triggered();

    void on_pushButtonSetDate_clicked();

    void on_actionXNVU_library_triggered();

    void on_dateEdit_userDateChanged(const QDate &date);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
