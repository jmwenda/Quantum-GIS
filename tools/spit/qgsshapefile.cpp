/***************************************************************************
                          qgsshapefile.cpp  -  description
                             -------------------
    begin                : Fri Dec 19 2003
    copyright            : (C) 2003 by Denis Antipov
    email                : 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/* $Id$ */

#include <qapplication.h>
#include <ogrsf_frmts.h>
#include <ogr_geometry.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>

#include "qgsdbfbase.h"
#include "cpl_error.h"
#include "qgsshapefile.h"

QgsShapeFile::QgsShapeFile(QString name){
  filename = (const char*)name;
  features = 0;
  OGRRegisterAll();
  ogrDataSource = OGRSFDriverRegistrar::Open((const char *) filename);
  if (ogrDataSource != NULL){
    valid = true;
    ogrLayer = ogrDataSource->GetLayer(0);
    features = ogrLayer->GetFeatureCount();
  }
  else
    valid = false;
  setDefaultTable();
}

QgsShapeFile::~QgsShapeFile(){
  delete ogrLayer;
  delete ogrDataSource;
  delete filename;
  delete geom_type;
}

int QgsShapeFile::getFeatureCount(){
  return features;
}

const char * QgsShapeFile::getFeatureClass(){
  OGRFeature *feat = ogrLayer->GetNextFeature();
  if(feat){
    OGRGeometry *geom = feat->GetGeometryRef();
    if(geom){
      geom_type = geom->getGeometryName();

      QString file(filename);
      file.replace(file.length()-3, 3, "dbf");
      // open the dbf file
      std::ifstream dbf((const char*)file, std::ios::in | std::ios::binary);
      // read header
      DbaseHeader dbh;
      dbf.read((char *)&dbh, sizeof(dbh));

      Fda fda;
      QString str_type = "varchar(";
      for(int field_count = 0, bytes_read = sizeof(dbh); bytes_read < dbh.size_hdr-1; field_count++, bytes_read += sizeof(fda)){
      	dbf.read((char *)&fda, sizeof(fda));
        switch(fda.field_type){
          case 'N': if((int)fda.field_decimal>0)
                      column_types.push_back("float");
                    else
                      column_types.push_back("int");          
                    break;
          case 'F': column_types.push_back("float");
                    break;                    
          case 'D': column_types.push_back("date");
                    break;
          case 'C': 
                    str_type= QString("varchar(%1)").arg(fda.field_length);
                    column_types.push_back(str_type);
                    break;
          case 'L': column_types.push_back("boolean");
                    break;
          default:
                    column_types.push_back("varchar(256)");
                    break;
        }
      }
      dbf.close();
      int numFields = feat->GetFieldCount();
      for(int n=0; n<numFields; n++)
        column_names.push_back(feat->GetFieldDefnRef(n)->GetNameRef());
      
    }else valid = false;
    delete feat;
  }else valid = false;
  
  ogrLayer->ResetReading();    
  return valid?geom_type:NULL;
}

bool QgsShapeFile::is_valid(){
  return valid;
}

const char * QgsShapeFile::getName(){
  return filename;
}

QString QgsShapeFile::getTable(){
  return table_name;
}

void QgsShapeFile::setTable(QString new_table){
  table_name = new_table;
}

void QgsShapeFile::setDefaultTable(){
  QString name(filename);
  name = name.section('/', -1);
  table_name = name.section('.', 0, 0);
}

bool QgsShapeFile::insertLayer(QString dbname, QString geom_col, QString srid, PgDatabase * conn, QProgressDialog * pro, bool &fin){
  connect(pro, SIGNAL(cancelled()), this, SLOT(cancelImport()));
  import_cancelled = false;
  bool result = true;
  QString message;

  QString query = "CREATE TABLE "+table_name+"(gid int4, ";
  for(int n=0; n<column_names.size(); n++){
    query += column_names[n].lower();
    query += " ";
    query += column_types[n];
    if(n < column_names.size() -1)
      query += ", ";
  }
  query += ")";  
  conn->ExecTuplesOk((const char *)query);
  message = conn->ErrorMessage();
  if(message != "") result = false;

  query = "SELECT AddGeometryColumn(\'" + dbname + "\', \'" + table_name + "\', \'"+geom_col+"\', " + srid +
    ", \'" + QString(geom_type) + "\', 2)";            
  if(result) conn->ExecTuplesOk((const char *)query);
  message = conn->ErrorMessage();
  if(message != "") result = false;

  //adding the data into the table
  for(int m=0;m<features && result; m++){
    if(import_cancelled){
      fin = true;
      break;
    }
    OGRFeature *feat = ogrLayer->GetNextFeature();
    if(feat){
      OGRGeometry *geom = feat->GetGeometryRef();
      if(geom){
        query = "INSERT INTO "+table_name+QString(" VALUES( %1, ").arg(m);
        
        int num = geom->WkbSize();
        char * geo_temp = new char[num*3];
        geom->exportToWkt(&geo_temp);
        QString geometry(geo_temp);

        QString quotes;
        for(int n=0; n<column_types.size(); n++){
          if(column_types[n] == "int" || column_types[n] == "float")
            quotes = " ";
          else
            quotes = "\'";
          query += quotes;
          query += QString(feat->GetFieldAsString(n));
          query += QString(quotes + ", ");

        }
        query += QString("GeometryFromText(\'")+QString(geometry)+QString("\', ")+srid+QString("))");
        if(result) conn->ExecTuplesOk((const char *)query);
        message = conn->ErrorMessage();
        if(message != "") result = false;

        pro->setProgress(pro->progress()+1);
        qApp->processEvents();
        delete[] geo_temp;
      }
      delete feat;
    }
  }
  ogrLayer->ResetReading();
  return result;
}

void QgsShapeFile::cancelImport(){
  import_cancelled = true;
}
