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

    public int getnumnodes()
    {
	return 5;
    }
    /*
    public int getnumattrs()
    {
    }
    public int getmaxattrs()
    {
    }
    */
    public long ghaddr()
    {
	return gh;
    }
    static
    {
	System.loadLibrary("Genders");
    }
}