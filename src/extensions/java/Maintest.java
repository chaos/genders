class Maintest
{
    private static void test (String filename)
    {
	Genders gh = new Genders(filename);
	System.out.println("Genders");
	System.out.println("NumNodes = " + gh.getnumnodes());
	System.out.println("Numattrs = " + gh.getnumattrs());
	System.out.println("Maxattrs = " + gh.getmaxattrs());
	System.out.println("Getnodname = " + gh.getnodename());

	String[] nodes;

	nodes = gh.getnodes();

	for (String tmp : nodes)
	    System.out.println("GetNodes All Node: " + tmp);
    }

    public static void main(String[] args)
    {
	/* test (null); */
	test ("./testgenders");
    }
}