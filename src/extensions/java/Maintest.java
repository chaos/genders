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

	nodes = gh.getnodes("mgmt");
	for (String tmp : nodes)
	    System.out.println("GetNodes mgmt Node: " + tmp);

	nodes = gh.getnodes("foobarlalala");
	for (String tmp : nodes)
	    System.out.println("GetNodes foobarlalala Node: " + tmp);

	nodes = gh.getnodes("cfhost");
	for (String tmp : nodes)
	    System.out.println("GetNodes cfhost Node: " + tmp);

	nodes = gh.getnodes("cfhost", "foobar");
	for (String tmp : nodes)
	    System.out.println("GetNodes cfhost=foobar  Node: " + tmp);

	nodes = gh.getnodes("cfhost", "hypei");
	for (String tmp : nodes)
	    System.out.println("GetNodes cfhost=hypei  Node: " + tmp);

	String attrs[];

	attrs = gh.getattr_all();
	for (String tmp : attrs)
	    System.out.println("Getattr All : " + tmp); 
    }

    public static void main(String[] args)
    {
	/* test (null); */
	test ("./testgenders");
    }
}