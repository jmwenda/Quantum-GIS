# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src
# Target is an application:  qgis

#LIBS += -L$/usr/local/lib -lgdal.1.1

SOURCES += main.cpp \
           qgisapp.cpp \
           qgisinterface.cpp \
           qgsmapcanvas.cpp \
           qgsmaplayer.cpp \
           qgsrasterlayer.cpp \
           qgsshapefilelayer.cpp \
           qgsrect.cpp \
           qgspoint.cpp \
           qgscoordinatetransform.cpp \
           qgslegend.cpp \
           qgslegenditem.cpp \
           qgssymbol.cpp \
           qgsmarkersymbol.cpp \
           qgslinesymbol.cpp \
           qgspolygonsymbol.cpp \
           qgslayerproperties.cpp \
           qgsidentifyresults.cpp \
           qgsattributetable.cpp \
           qgsattributetabledisplay.cpp \
           qgsrenderer.cpp \
           qgsrenderitem.cpp \
           qgsprojectio.cpp \
           qgisiface.cpp \
	   qgspluginmanager.cpp \
	   qgspluginitem.cpp \
	   qgsfeature.cpp \
	   qgsfeatureattribute.cpp
HEADERS += qgisapp.h \
           qgisinterface.h \
           qgisappbase.ui.h \
           qgsattributetablebase.ui.h \
           qgsdatasource.h \
           qgsmapcanvas.h \
           qgsmaplayer.h \
           qgsrasterlayer.h \
           qgsshapefilelayer.h \
           qgstable.h \
           qgsrect.h \
           qgspoint.h \
           qgscoordinatetransform.h \
           qgssymbol.h \
           qgsmarkersymbol.h \
           qgslinesymbol.h \
           qgspolygonsymbol.h \
           qgslegend.h \
           qgslegenditem.h \
           qgslayerproperties.h \
           qgsidentifyresults.h \
           qgsattributetable.h \
           qgsattributetabledisplay.h \
           qgsrenderer.h \
           qgsrenderitem.h \
           qgsprojectio.h \
           qgisiface.h \
	   qgspluginmanager.h \
	   qgspluginitem.h \
	   qgsmaplayerinterface.h \
	   qgsfeature.h \
	   qgsfeatureattribute.h
FORMS += qgisappbase.ui \
         qgslegenditembase.ui \
         qgsabout.ui \
         qgslayerpropertiesbase.ui \
         qgsidentifyresultsbase.ui \
         qgsattributetablebase.ui \
         qgspluginmanagerbase.ui  \
		 qgsmessageviewer.ui
TEMPLATE = app 
CONFIG += debug \
          warn_on \
          qt \
          thread 
TARGET = qgis 

#.............................
# GDAL/OGR configuration
#.............................
message(Configuring GDAL)
GDALCONFIG = $$system(which gdal-config)
isEmpty(GDALCONFIG) {
	error("gdal-config not found in PATH. Check GDAL installation.")
}
# check to see if ogr enabled
OGR = $$system(gdal-config --ogr-enabled)
message("OGR enabled - $$OGR")
LIBS+= $$system(gdal-config --libs)
GDALINC = $$system(gdal-config --cflags)
INCLUDEPATH += $$GDALINC

# conditional tests for optional modules

#.............................
#PostgreSQL support
#.............................
contains (DEFINES, POSTGRESQL){
 message ("Checking PostgreSQL environment")
}
contains ( DEFINES, POSTGRESQL ){
MYPGSQL=$$(PGSQL)
count(MYPGSQL, 1){
message ("PGSQL environment variable is defined")


  message ( "Configuring to build with PostgreSQL support" )
  LIBS += -L$(PGSQL)/lib -lpq++
  INCLUDEPATH += $(PGSQL)/include
  DEFINES += HAVE_NAMESPACE_STD HAVE_CXX_STRING_HEADER DLLIMPORT=""
  SOURCES += qgsdatabaselayer.cpp \
             qgsdbsourceselect.cpp \
             qgsnewconnection.cpp 
  HEADERS += qgsdbsourceselectbase.ui.h \
             qgsdatabaselayer.h \
             qgsdbsourceselect.h \
             qgsnewconnection.h 
  FORMS += qgsdbsourceselectbase.ui \
           qgsnewconnectionbase.ui 
}
count(MYPGSQL, 0){
message ("PGSQL environment variable is not defined. PostgreSQL excluded from build")
message ("To build with PostgreSQL support set PGSQL to point to your Postgres installation")
}
}
message ("Configuration complete, type make to build qgis")
