/***************************************************************************
    oracleselectgui.h
    -------------------
    begin                : Oracle Spatial Plugin
    copyright            : (C) Ivan Lucena
    email                : ivan.lucena@pmldnet.com
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/* $Id$ */

#ifndef OraclePluginGUI_H
#define OraclePluginGUI_H

// Qt Designer Includes
#include "ui_qgsselectgeorasterbase.h"

//Qt includes
#include <QSettings>
#include <QMessageBox>
#include <QInputDialog>

// QGIS Includes
#include <qgisinterface.h>
#include <qgsmapcanvas.h>

class QgsOracleSelectGeoraster : public QDialog, private Ui::SelectGeoRasterBase
{
    Q_OBJECT

public:
    QgsOracleSelectGeoraster(QWidget* parent, QgisInterface* iface, Qt::WFlags fl = 0);
    ~QgsOracleSelectGeoraster();

private:
    QgisInterface* mIface;
    QString mUri;

private:
    void addNewConnection();
    void editConnection();
    void deleteConnection();
    void populateConnectionList();
    void connectToServer();
    void showHelp();
    void setConnectionListPosition();
    void showSelection( const QString & line );

public slots:

    void on_btnNew_clicked()
    {
        addNewConnection();
    };

    void on_btnEdit_clicked()
    {
        editConnection();
    };

    void on_btnDelete_clicked()
    {
        deleteConnection();
    };

    void on_btnConnect_clicked()
    {
        connectToServer();
    };

    void on_listWidget_clicked( QModelIndex Index )
    {
        if( lineEdit->text() == listWidget->currentItem()->text() )
        {
            showSelection( lineEdit->text() );
        }
        else
        {
            lineEdit->setText( listWidget->currentItem()->text() );
        }
    }

    void on_btnAdd_clicked()
    {
        showSelection( lineEdit->text() );
    };

};

#endif
