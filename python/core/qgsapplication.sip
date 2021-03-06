
class QgsApplication : QApplication
{
%TypeHeaderCode
#include <qgsapplication.h>
%End

%TypeCode
// Convert a Python argv list to a conventional C argc count and argv array.
static char **qtgui_ArgvToC(PyObject *argvlist, int &argc)
{
    char **argv;

    argc = PyList_GET_SIZE(argvlist);

    // Allocate space for two copies of the argument pointers, plus the
    // terminating NULL.
    if ((argv = (char **)sipMalloc(2 * (argc + 1) * sizeof (char *))) == NULL)
        return NULL;

    // Convert the list.
    for (int a = 0; a < argc; ++a)
    {
        char *arg;

        // Get the argument and allocate memory for it.
        if ((arg = PyString_AsString(PyList_GET_ITEM(argvlist, a))) == NULL ||
            (argv[a] = (char *)sipMalloc(strlen(arg) + 1)) == NULL)
            return NULL;

        // Copy the argument and save a pointer to it.
        strcpy(argv[a], arg);
        argv[a + argc + 1] = argv[a];
    }

    argv[argc + argc + 1] = argv[argc] = NULL;

    return argv;
}


// Remove arguments from the Python argv list that have been removed from the
// C argv array.
static void qtgui_UpdatePyArgv(PyObject *argvlist, int argc, char **argv)
{
    for (int a = 0, na = 0; a < argc; ++a)
    {
        // See if it was removed.
        if (argv[na] == argv[a + argc + 1])
            ++na;
        else
            PyList_SetSlice(argvlist, na, na + 1, NULL);
    }
}
%End



  public:
    //QgsApplication(int argc, char ** argv, bool GUIenabled);
    QgsApplication(SIP_PYLIST argv, bool GUIenabled, QString customConfigPath = QString() ) /PostHook=__pyQtQAppHook__/ [(int &argc, char **argv, bool GUIenabled, QString customConfigPath = QString() )];
%MethodCode
        // The Python interface is a list of argument strings that is modified.
        
        int argc;
        char **argv;
        
        // Convert the list.
        if ((argv = qtgui_ArgvToC(a0, argc)) == NULL)
            sipIsErr = 1;
        else
        {
            // Create it now the arguments are right.
            static int nargc = argc;
        
            sipCpp = new sipQgsApplication(nargc, argv, a1, *a2);
        
            // Now modify the original list.
            qtgui_UpdatePyArgv(a0, argc, argv);
        }
%End

    virtual ~QgsApplication();

    /** Set the active theme to the specified theme.
     * The theme name should be a single word e.g. 'default','classic'.
     * The theme search path usually will be pkgDataPath + "/themes/" + themName + "/"
     * but plugin writers etc can use themeName() as a basis for searching
     * for resources in their own datastores e.g. a Qt4 resource bundle.
     * @Note A basic test will be carried out to ensure the theme search path
     * based on the supplied theme name exists. If it does not the theme name will 
     * be reverted to 'default'.
     */
    static void setThemeName(const QString theThemeName);

    /** Set the active theme to the specified theme.
     * The theme name should be a single word e.g. 'default','classic'.
     * The theme search path usually will be pkgDataPath + "/themes/" + themName + "/"
     * but plugin writers etc can use this method as a basis for searching
     * for resources in their own datastores e.g. a Qt4 resource bundle.
     */
    static const QString themeName();

    //! Returns the path to the authors file.
    static const QString authorsFilePath();

    /** Returns the path to the contributors file.
     * Contributors are people who have submitted patches
     * but don't have svn write access.
     * @note this function was added in version 1.3 */
    static const QString contributorsFilePath();

    /**Returns the path to the sponsors file.
      @note this function was added in version 1.2*/
    static const QString sponsorsFilePath();

    /** Returns the path to the donors file.
      @note this function was added in version 1.2*/
    static const QString donorsFilePath();

    /**
     * Returns the path to the sponsors file.
     * @note This was added in QGIS 1.1
     */
    static const QString translatorsFilePath();

    //! Returns the path to the developer image directory.
    static const QString developerPath();

    //! Returns the path to the help application.
    static const QString helpAppPath();

    //! Returns the path to the translation directory.
    static const QString i18nPath();

    //! Returns the path to the master qgis.db file.
    static const QString qgisMasterDbFilePath();

    //! Returns the path to the settings directory in user's home dir
    static const QString qgisSettingsDirPath();
    
    //! Returns the path to the user qgis.db file.
    static const QString qgisUserDbFilePath();

    //! Returns the path to the splash screen image directory.
    static const QString splashPath();

    //! Returns the path to the icons image directory.
    static const QString iconsPath();

    //! Returns the path to the srs.db file.
    static const QString srsDbFilePath();

    //! Returns the paths to the svg directories.
    //! @note added in 1.4
    static const QStringList svgPaths();

    //! Returns the path to the applications svg paths
    //! @deprecated use svgPaths()
    static const QString svgPath() /Deprecated/;

    //! Returns the path to the application prefix directory.
    static const QString prefixPath();

    //! Returns the path to the application plugin directory.
    static const QString pluginPath();

    //! Returns the common root path of all application data directories.
    static const QString pkgDataPath();

    //! Returns the path to the currently active theme directory.
    static const QString activeThemePath(); 

    //! Returns the path to the default theme directory.
    static const QString defaultThemePath(); 
    
    //! Returns path to the desired icon file.
    //! First it tries to use the active theme path, then default theme path
    //! @note Added in 1.5
    static QString iconPath( QString iconFile );

    //! Returns the path to user's style. Added in QGIS 1.4
    static const QString userStyleV2Path();

    //! Returns the path to default style (works as a starting point). Added in QGIS 1.4
    static const QString defaultStyleV2Path();

    //! Returns the path containing qgis_core, qgis_gui, qgispython (and other) libraries
    //! @note Added in 2.0
    static const QString libraryPath();

    //! Returns the path with utility executables (help viewer, crssync, ...)
    //! @note Added in 2.0
    static const QString libexecPath();

    //! Alters prefix path - used by 3rd party apps
    static void setPrefixPath(const QString thePrefixPath, bool useDefaultPaths = FALSE);
    
    //! Alters plugin path - used by 3rd party apps
    static void setPluginPath(const QString thePluginPath);

    //! Alters pkg data path - used by 3rd party apps
    static void setPkgDataPath(const QString thePkgDataPath);

    //! Alters default svg paths - used by 3rd party apps. Added in QGIS 1.5
    static void setDefaultSvgPaths( const QStringList& pathList );
    
    //! loads providers
    static void initQgis();

    //! deletes provider registry and map layer registry
    static void exitQgis();
    
    /** constants for endian-ness */
    enum endian_t
    {
      XDR = 0,  // network, or big-endian, byte order
      NDR = 1   // little-endian byte order
    };
    
    //! Returns whether this machine uses big or little endian
    static endian_t endian();

    /** \brief get a standard css style sheet for reports.
     * Typically you will use this method by doing:
     * QString myStyle = QgsApplication::reportStyleSheet();
     * textBrowserReport->document()->setDefaultStyleSheet(myStyle);
     * @return QString containing the CSS 2.1 compliant stylesheet.
     * @note you can use the special Qt extensions too, for example
     * the gradient fills for backgrounds.
     */
    static QString reportStyleSheet();

    /** Convenience function to get a summary of the paths used in this 
     * application instance useful for debugging mainly.*/
    static QString showSettings();

    /** Register OGR drivers ensuring this only happens once.
     * This is a workaround for an issue with older gdal versions that
     * caused duplicate driver name entries to appear in the list 
     * of registered drivers when QgsApplication::registerOgrDrivers was called multiple 
     * times.
     */
    static void registerOgrDrivers();

    /**Converts absolute path to path relative to target
      @note: this method was added in version 1.6*/
    static QString absolutePathToRelativePath( QString apath, QString targetPath );
    /**Converts path relative to target to an absolute path
      @note: this method was added in version 1.6*/
    static QString relativePathToAbsolutePath( QString rpath, QString targetPath );

    /** Indicates whether running from build directory (not installed)
       @note added in 2.0 */
    static bool isRunningFromBuildDir();
    /** Returns path to the source directory. Valid only when running from build directory
        @note added in 2.0 */
    static QString buildSourcePath();
    /** Returns path to the build output directory. Valid only when running from build directory
        @note added in 2.0 */
    static QString buildOutputPath();

};

