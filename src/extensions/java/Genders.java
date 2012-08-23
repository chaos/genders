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

    static
    {
	System.loadLibrary("Genders");
    }
}