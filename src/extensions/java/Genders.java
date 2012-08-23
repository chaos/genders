class Genders
{
    private long gh = 0;

    private native int genders_constructor_default();

    private native int genders_constructor_filename(String filename);

    public Genders()
    {
	if (genders_constructor_default() < 0)
	    {
		System.out.println("genders_constructor_default");
	    }
    }

    public Genders(String filename)
    {
	if (genders_constructor_filename(filename) < 0)
	    {
		System.out.println("genders_constructor_filename");
	    }
    }

    public native int getnumnodes();

    public native int getnumattrs();

    public native int getmaxattrs();

    public long ghaddr()
    {
	return gh;
    }

    static
    {
	System.loadLibrary("Genders");
    }
}