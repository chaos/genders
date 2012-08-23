class Genders
{
    private long gh = 0;

    private native int genders_handle_setup();

    public Genders()
    {
	if (genders_handle_setup() < 0)
	    {
		System.out.println("genders_handle_setup: error");
	    }
	System.out.println("gh = " + gh); 
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