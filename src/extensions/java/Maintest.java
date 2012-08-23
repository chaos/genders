class Maintest
{
    private static void test (String filename)
    {
	Genders gh = new Genders(filename);
	System.out.println("Genders Default");
	System.out.println("NumNodes = " + gh.getnumnodes());
	System.out.println("Numattrs = " + gh.getnumattrs());
	System.out.println("Maxattrs = " + gh.getmaxattrs());
    }

    public static void main(String[] args)
    {
	test (null);
	test ("./testgenders");
    }
}