package gov.llnl.lc.chaos;

/**
 * Class Name: Genders
 *
 * Java JNI extensions wrapper around libgenders C library.  Most
 * library functions behave similarly to C api functions with minor
 * exceptions documented below.
 */
public class Genders
{
    private long gh_addr = 0;

    private native int genders_constructor(String filename);

    /**
     * Creates a Genders object, loading the default genders database
     */
    public Genders()
    {
	genders_constructor(null);
    }

    /**
     * Creates a Genders object, loading the specified genders database
     */
    public Genders(String filename)
    {
	genders_constructor(filename);
    }

    /**
     * Returns number of nodes parsed in the genders database
     *
     * @return number of nodes on success
     * @throws GendersException on error
     */
    public native int getnumnodes() throws GendersException;

    /**
     * Returns number of attributes parsed in the genders database
     *
     * @return number of attributes on success
     * @throws GendersException on error
     */
    public native int getnumattrs() throws GendersException;

    /**
     * Returns maximum number of attributes of any one node parsed in
     * the genders database
     *
     * @return maximum number of attributes of any one node on success
     * @throws GendersException on error
     */
    public native int getmaxattrs() throws GendersException;

    public native String getnodename() throws GendersException;

    public native String[] getnodes() throws GendersException;

    public native String[] getnodes(String attr) throws GendersException;

    public native String[] getnodes(String attr, String val) throws GendersException;

    public native String[] getattr() throws GendersException;

    public native String[] getattr(String node) throws GendersException;

    public native String[] getattr_all() throws GendersException;

    public native String getattrval(String attr) throws GendersException;

    public native String getattrval(String node, String attr) throws GendersException;

    public native boolean testattr(String attr) throws GendersException;

    public native boolean testattr(String node, String attr) throws GendersException;

    public native boolean testattrval(String attr, String val) throws GendersException;

    public native boolean testattrval(String node, String attr, String val) throws GendersException;

    public native boolean isnode(String node) throws GendersException;

    public native boolean isattr(String attr) throws GendersException;

    public native boolean isattrval(String attr, String val) throws GendersException;

    public native String[] query(String query) throws GendersException;

    public native boolean testquery(String query) throws GendersException;

    public native boolean testquery(String node, String query) throws GendersException;

    public native int parse() throws GendersException;

    public native int parse(String filename) throws GendersException;

    public native void cleanup();

    static
    {
	System.loadLibrary("Gendersjni");
    }
}
