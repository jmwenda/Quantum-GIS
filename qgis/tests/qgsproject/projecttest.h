#include <iostream>
#include <fstream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <qgsproject.h>

//qt includes
#include <qstring.h>

class ProjectTest : public CppUnit::TestFixture 
{ 
    CPPUNIT_TEST_SUITE( ProjectTest );

    CPPUNIT_TEST( testFileName );
    CPPUNIT_TEST( testTitle );
    CPPUNIT_TEST( testMapUnits );
    CPPUNIT_TEST( testDirtyFlag );
    CPPUNIT_TEST( readNullEntries );

    CPPUNIT_TEST_SUITE_END();

  public: 

    /** 
        Setup the common test members, etc
    */
    void setUp()
    {
        mFile = "test.project";
        mTitle = "test title";
        mScope = "project_test";

        mNumValueKey = "/values/num";

        mDoubleValueKey = "/values/double";

        mBoolValueKey = "/values/bool";

        mStringValueKey = "/values/string";

        mStringListValueKey = "/values/stringlist";


        mNumValueConst = 42;

        mDoubleValueConst = 12345.6789;

        mBoolValueConst = true;

        mStringValueConst = "Test String";

        mStringListValueConst += "first";
        mStringListValueConst += "second";
        mStringListValueConst += "third";
    } // setUp

    

    void testFileName()
    {
        QgsProject::instance()->dirty( false );
        QgsProject::instance()->filename( mFile );

        CPPUNIT_ASSERT( mFile == QgsProject::instance()->filename() );
        CPPUNIT_ASSERT( QgsProject::instance()->dirty() );
    } // testFileName

    

    void testTitle()
    {
        QgsProject::instance()->dirty( false );
        QgsProject::instance()->title( mTitle );

        CPPUNIT_ASSERT( mTitle == QgsProject::instance()->title() );
        CPPUNIT_ASSERT( QgsProject::instance()->dirty() );
    } // testTitle


    void testMapUnits()
    {
        QgsProject::instance()->dirty( false );
        QgsProject::instance()->mapUnits( QgsScaleCalculator::METERS );
        CPPUNIT_ASSERT( QgsScaleCalculator::METERS == QgsProject::instance()->mapUnits() );
        CPPUNIT_ASSERT( QgsProject::instance()->dirty() );

        QgsProject::instance()->dirty( false );
        QgsProject::instance()->mapUnits( QgsScaleCalculator::FEET );
        CPPUNIT_ASSERT( QgsScaleCalculator::FEET == QgsProject::instance()->mapUnits() );
        CPPUNIT_ASSERT( QgsProject::instance()->dirty() );

        QgsProject::instance()->dirty( false );
        QgsProject::instance()->mapUnits( QgsScaleCalculator::DEGREES );
        CPPUNIT_ASSERT( QgsScaleCalculator::DEGREES == QgsProject::instance()->mapUnits() );
        CPPUNIT_ASSERT( QgsProject::instance()->dirty() );
    } // testMapUnits



    void testDirtyFlag()
    {
        QgsProject::instance()->dirty( true );
        CPPUNIT_ASSERT( QgsProject::instance()->dirty() );

        QgsProject::instance()->dirty( false );
        CPPUNIT_ASSERT( ! QgsProject::instance()->dirty() );
    } // testDirtyFlag

    
    /**
       Reading entries that are known not to exist should fail and use default
       values.
     */
    void readNullEntries()
    {
        bool status;


        bool b = QgsProject::instance()->readBoolEntry( mScope, mBoolValueKey, false, &status);

        CPPUNIT_ASSERT( false == b && ! status );


        int i = QgsProject::instance()->readNumEntry( mScope, mNumValueKey, 13, &status);

        CPPUNIT_ASSERT( 13 == i && ! status );


        double d = QgsProject::instance()->readDoubleEntry( mScope, mDoubleValueKey, 99.0, &status);

        CPPUNIT_ASSERT( 99.0 == d && ! status );


        QString s = QgsProject::instance()->readEntry( mScope, mStringValueKey, "FOO", &status);

        CPPUNIT_ASSERT( "FOO" == s && ! status );


        QStringList sl = QgsProject::instance()->readListEntry( mScope, mStringListValueKey, &status);

        CPPUNIT_ASSERT( sl.empty() && ! status );

    } // readNullEntries


private:

    /// file name for project file
    QString mFile;

    /// test project title
    QString mTitle;

    /// test project scope
    QString mScope;

    /// num value key
    QString mNumValueKey;

    /// double value key
    QString mDoubleValueKey;

    /// bool value key
    QString mBoolValueKey;

    /// string value key
    QString mStringValueKey;

    /// string list value key
    QString mStringListValueKey;

    /// num value const
    int mNumValueConst;

    /// double value const
    double mDoubleValueConst;

    /// bool value const
    bool mBoolValueConst;

    /// string value const
    QString mStringValueConst;

    /// string list value const
    QStringList mStringListValueConst;


}; // class ProjectTest

