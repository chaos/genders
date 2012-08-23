class Genders
{
    private long gh_addr = 0;

    private native int genders_constructor(String filename);

    public Genders()
    {
	if (genders_constructor(null) < 0)
	    {
		System.out.println("genders_constructor_default");
	    }
    }

    public Genders(String filename)
    {
	if (genders_constructor(filename) < 0)
	    {
		System.out.println("genders_constructor_filename");
	    }
    }

    public native int getnumnodes();

    public native int getnumattrs();

    public native int getmaxattrs();

    public native String getnodename();

    public native String[] getnodes();

    public native String[] getnodes(String attr);

    public native String[] getnodes(String attr, String val);

    // getattr

    public native String[] getattr_all();

    public native String getattrval(String attr);

    public native String getattrval(String node, String attr);

    public native boolean testattr(String attr);

    public native boolean testattr(String node, String attr);

    public native boolean testattrval(String attr, String val);

    public native boolean testattrval(String node, String attr, String val);

    public native boolean isnode(String node);

    public native boolean isattr(String attr);

    public native boolean isattrval(String attr, String val);

    public native String[] query(String query);

    public native boolean testquery(String query);

    public native boolean testquery(String node, String query);

    public native int parse();

    public native int parse(String filename);

    static
    {
	System.loadLibrary("Genders");
    }
}