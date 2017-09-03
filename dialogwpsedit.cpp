#include "dialogwpsedit.h"
#include "ui_dialogwpsedit.h"
#include "qlistwidgetitemdata.h"
#include "dialogwaypointedit.h"
#include "XFMS_DATA.h"

DialogWPSEdit::DialogWPSEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWPSEdit)
{
    ui->setupUi(this);

    initializeList(NULL);
}

DialogWPSEdit::~DialogWPSEdit()
{
    delete ui;
}

void DialogWPSEdit::initializeList(NVUPOINT* select)
{
    QListWidgetItemData* cItem;
    NVUPOINT* p;
    int sIndex = -1;

    ui->listWPS->clear();
    for(int i=0; i<XFMS_DATA::lXNVU.size(); i++)
    {
        p = XFMS_DATA::lXNVU[i];
        cItem = new QListWidgetItemData();
        cItem->nvupoint = p;
        cItem->setText(p->name + (!p->country.isEmpty() ? (" [" + p->country + "]") : " " + (!p->name2.isEmpty() ? "  " + p->name2 : "")));
        ui->listWPS->addItem(cItem);
        if(select == p) sIndex = i;
    }

    if(sIndex>=0) ui->listWPS->setCurrentRow(sIndex);
}

void DialogWPSEdit::setWaypointDescription(const NVUPOINT* wp)
{
    ui->labelIWPName2->setText("");
    ui->labelWPType->setText("");
    ui->labelWPType2->setText("");
    ui->labelWPMagVar->setText("");
    ui->labelWPLatlon->setText("");
    ui->labelWPNote->setText("");

    if(!wp) return;

    QString qstr;
    qstr = WAYPOINT::getTypeStr(wp);
    ui->labelWPType2->setText(qstr);

    qstr = wp->name;
    if(!wp->country.isEmpty()) qstr = qstr + " [" + wp->country + "]";

    if(wp->type == WAYPOINT::TYPE_NDB ||
       wp->type == WAYPOINT::TYPE_RSBN)
    {
        qstr = qstr + "  Ch" + QString::number((int) wp->freq);
    }//if
    else if(wp->type == WAYPOINT::TYPE_VOR ||
            wp->type == WAYPOINT::TYPE_DME ||
            wp->type == WAYPOINT::TYPE_VORDME ||
            wp->type == WAYPOINT::TYPE_ILS ||
            wp->type == WAYPOINT::TYPE_TACAN ||
            wp->type == WAYPOINT::TYPE_VORTAC)
    {
        qstr = qstr + "  " + QString::number(wp->freq, 'f', 3);
    }//if
    ui->labelWPType->setText(qstr);

    if(wp->type == WAYPOINT::TYPE_AIRWAY && wp->data)
    {
        AIRWAY* ats = (AIRWAY*) wp->data;
        ui->labelIWPName2->setText("[" + ats->lATS[0]->name + "] ---> [" + ats->lATS[ats->lATS.size()-1]->name + "]");
    }
    else if(!wp->name2.isEmpty()) ui->labelIWPName2->setText(wp->name2);

    if(wp->type == WAYPOINT::TYPE_AIRWAY && wp->data)
    {
        AIRWAY* ats = (AIRWAY*) wp->data;
        ui->labelWPLatlon->setText("Fixes: " + QString::number(ats->lATS.size()) + "    Dist: " + QString::number(ats->distance, 'f', 1) + " KM");
    }
    else
    {
        double l1, l2;
        l1 = fabs(modf(wp->latlon.x, &l2)*60.0);
        int i2 = (int) fabs(l2);
        qstr = "";
        qstr = qstr + "Lat:   " + (wp->latlon.x < 0 ? "S" : "N") + (i2<10 ? "0" : "") + QString::number(i2) + "*" + (l1<10 ? "0" : "") + QString::number(l1, 'f', 2) + "       ";
        l1 = fabs(modf(wp->latlon.y, &l2)*60.0);
        i2 = (int) fabs(l2);
        qstr = qstr + "Lon:  " + (wp->latlon.y < 0 ? "W" : "E") + (i2<100 ? (i2<10 ? "00" : "0") : "") + QString::number(i2) + "*" + (l1<10 ? "0" : "") + QString::number(l1, 'f', 2);
        ui->labelWPLatlon->setText(qstr);
    }//else


    if(wp->type==WAYPOINT::TYPE_AIRWAY)
    {
        ui->labelWPMagVar->setText("CLICK TO SHOW WAYPOINTS");
    }
    else ui->labelWPMagVar->setText("Magnetic Declination: " + QString::number(wp->MD, 'f', 1));

    ui->labelWPNote->setText("Source: " + WAYPOINT::getOriginStr(wp->wpOrigin));
}


void DialogWPSEdit::on_listWPS_itemSelectionChanged()
{
    int c = ui->listWPS->currentRow();

    QListWidgetItemData* iD = (QListWidgetItemData*) ui->listWPS->item(c);

    if(iD) setWaypointDescription(iD->nvupoint);
    else setWaypointDescription(NULL);
}

void DialogWPSEdit::on_pushButton_Edit_clicked()
{
    int c = ui->listWPS->currentRow();

    QListWidgetItemData* iD = (QListWidgetItemData*) ui->listWPS->item(c);

    if(!iD) return;

    DialogWaypointEdit dEdit(iD->nvupoint, true);
    const int rv = dEdit.exec();

    switch(rv)
    {
        case DialogWaypointEdit::SAVE:
            dEdit.newPoint->wpOrigin = WAYPOINT::ORIGIN_XNVU;
            XFMS_DATA::addXNVUWaypoint(dEdit.newPoint);
            XFMS_DATA::removeXNVUWaypoint(iD->nvupoint);
            delete iD->nvupoint;
            iD->nvupoint = dEdit.newPoint;
            initializeList(iD->nvupoint);
        break;

        case DialogWaypointEdit::ADD_XNVU:
            XFMS_DATA::addXNVUWaypoint(dEdit.newPoint);
            initializeList(dEdit.newPoint);
        break;
    }
}


void DialogWPSEdit::on_pushButton_Delete_clicked()
{
    int c = ui->listWPS->currentRow();
    QListWidgetItemData* iD = (QListWidgetItemData*) ui->listWPS->item(c);
    if(!iD) return;
    XFMS_DATA::removeXNVUWaypoint(iD->nvupoint);
    initializeList(NULL);
}

void DialogWPSEdit::on_pushButton_CreateNew_clicked()
{
    DialogWaypointEdit dEdit(NULL, false);
    const int rv = dEdit.exec();

    switch(rv)
    {
        case DialogWaypointEdit::ADD_XNVU:
            NVUPOINT* nP = new NVUPOINT(*dEdit.newPoint);
            XFMS_DATA::addXNVUWaypoint(nP);
            initializeList(nP);
        break;
    }
}


void DialogWPSEdit::on_pushButton_clicked()
{
    close();
}
