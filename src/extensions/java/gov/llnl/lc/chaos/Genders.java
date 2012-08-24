package gov.llnl.lc.chaos;

public class Genders
{
    private long gh_addr = 0;

    private native int genders_constructor(String filename);

    public Genders()
    {
	genders_constructor(null);
    }

    public Genders(String filename)
    {
	genders_constructor(filename);
    }

    public native int getnumnodes() throws GendersException;

    public native int getnumattrs() throws GendersException;

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
	System.loadLibrary("Genders");
    }
}
