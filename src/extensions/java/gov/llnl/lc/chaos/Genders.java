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
     * @throws GendersException on error
     */
    public native int getnumnodes() throws GendersException;

    /**
     * Returns number of attributes parsed in the genders database
     *
     * @throws GendersException on error
     */
    public native int getnumattrs() throws GendersException;

    /**
     * Returns maximum number of attributes of any one node parsed in
     * the genders database
     *
     * @throws GendersException on error
     */
    public native int getmaxattrs() throws GendersException;

    /**
     * Returns the current node you are on, in shortened hostname
     * format.
     *
     * @throws GendersException on error
     */
    public native String getnodename() throws GendersException;

    /**
     * Returns all the nodes in the genders database
     *
     * @return 
     * @throws GendersException on error
     */ 
    public native String[] getnodes() throws GendersException;

    /**
     * Returns all the nodes with the specified attribute
     *
     * @throws GendersException on error
     */ 
    public native String[] getnodes(String attr) throws GendersException;

    /**
     * Returns all the nodes with the specified attribute and value
     *
     * @throws GendersException on error
     */ 
    public native String[] getnodes(String attr, String val) throws GendersException;

    /**
     * Returns all the attributes of the node you are running on
     *
     * @throws GendersException on error
     */ 
    public native String[] getattr() throws GendersException;

    /**
     * Returns all the attributes of the specified node
     *
     * @throws GendersException on error
     */ 
    public native String[] getattr(String node) throws GendersException;

    /**
     * Returns all of the attributes in the genders database
     *
     * @throws GendersException on error
     */ 
    public native String[] getattr_all() throws GendersException;

    /**
     * Returns the value of the specified attribute on the current
     * node you are running on
     *
     * @throws GendersException on error
     */ 
    public native String getattrval(String attr) throws GendersException;

    /**
     * Returns the value of the specified attribute on the specified
     * node.  May be an empty string if the attribute contains no
     * value.
     *
     * @throws GendersException on error
     */ 
    public native String getattrval(String node, String attr) throws GendersException;

    /**
     * Tests if the current node has the specified attribute
     *
     * @throws GendersException on error
     */ 
    public native boolean testattr(String attr) throws GendersException;

    /**
     * Tests if the specified node has the specified attribute
     *
     * @throws GendersException on error
     */ 
    public native boolean testattr(String node, String attr) throws GendersException;

    /**
     * Tests if the current node has the specified attribute and value.
     * 
     * @throws GendersException on error
     */ 
    public native boolean testattrval(String attr, String val) throws GendersException;

    /**
     * Tests if the specified node has the specified attribute and value.
     *
     * @throws GendersException on error
     */ 
    public native boolean testattrval(String node, String attr, String val) throws GendersException;

    /**
     * Tests if the specified node exists in the genders database
     * 
     * @throws GendersException on error
     */ 
    public native boolean isnode(String node) throws GendersException;

    /**
     * Tests if the specified attribute exists in the genders database
     *
     * @throws GendersException on error
     */ 
    public native boolean isattr(String attr) throws GendersException;

    /**
     * Tests if the specified value exists in the genders database
     *
     * @throws GendersException on error
     */ 
    public native boolean isattrval(String attr, String val) throws GendersException;

    /**
     * Returns nodes specified via the specified query.  Signify union
     * with '||', intersection with '&&', * difference with '--', and
     * complement with '~'.  Operations are performed left to
     * right. Parentheses can be used to change the order of
     * operations.
     *
     * @throws GendersException on error
     */ 
    public native String[] query(String query) throws GendersException;

    /**
     * Test if the current node meets the conditions of the specified query.
     *
     * @throws GendersException on error
     */ 
    public native boolean testquery(String query) throws GendersException;

    /**
     * Test if the specified node meets the conditions of the specified query.
     *
     * @throws GendersException on error
     */ 
    public native boolean testquery(String node, String query) throws GendersException;

    /**
     * Parse the default genders database and output debugging
     * information parse errors.
     *
     * @return Number of errors found (0 for none)
     * @throws GendersException on error
     */ 
    public native int parse() throws GendersException;

    /**
     * Parse the specified genders database and output debugging
     * information parse errors.
     *
     * @return Number of errors found (0 for none)
     * @throws GendersException on error
     */ 
    public native int parse(String filename) throws GendersException;

    /**
     * Cleans up allocated memory.  Must be called to free memory from
     * underlying calls.  After this method is called, all genders
     * methods above cannot be called and will result in errors.
     */ 
    public native void cleanup();

    static
    {
	System.loadLibrary("Gendersjni");
    }
}
