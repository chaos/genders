class Maintest
{
    private static void test (String filename)
    {
	Genders gh = new Genders(filename);

	try {
	    System.out.println("NumNodes = " + gh.getnumnodes());
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}
	try {
	    System.out.println("Numattrs = " + gh.getnumattrs());
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}
	try {
	    System.out.println("Maxattrs = " + gh.getmaxattrs());
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}
	try {
	    System.out.println("Getnodname = " + gh.getnodename());
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	String[] nodes;

	try {
	    nodes = gh.getnodes();
	    for (String tmp : nodes)
		System.out.println("GetNodes All Node: " + tmp);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    nodes = gh.getnodes("mgmt");
	    for (String tmp : nodes)
		System.out.println("GetNodes mgmt Node: " + tmp);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    nodes = gh.getnodes("foobarlalala");
	    for (String tmp : nodes)
		System.out.println("GetNodes foobarlalala Node: " + tmp);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    nodes = gh.getnodes("cfhost");
	    for (String tmp : nodes)
		System.out.println("GetNodes cfhost Node: " + tmp);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    nodes = gh.getnodes("cfhost", "foobar");
	    for (String tmp : nodes)
		System.out.println("GetNodes cfhost=foobar  Node: " + tmp);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    nodes = gh.getnodes("cfhost", "hypei");
	    for (String tmp : nodes)
		System.out.println("GetNodes cfhost=hypei  Node: " + tmp);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	String attrs[];
	String vals[];

	try {
	    attrs = gh.getattr();
	    for (String tmp : attrs)
		System.out.println("getattr : " + tmp); 
	}
	catch (Exception e) {
	    System.out.println("getattr : " + e);
	}

	try {
	    attrs = gh.getattr("hype355");
	    for (String tmp : attrs)
		System.out.println("getattr hype355: " + tmp); 
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    attrs = gh.getattr_all();
	    for (String tmp : attrs)
		System.out.println("Getattr All : " + tmp);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	boolean b;

	try {
	    b = gh.isnode("foobar");
	    System.out.println("Isnode foobar : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.isnode("hype355");
	    System.out.println("Isnode hype355 : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.isnode(null);
	    System.out.println("Isnode null : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.isattr("foobar");
	    System.out.println("Isattr foobar : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.isattr("mgmt");
	    System.out.println("Isattr mgmt : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.isattrval("cfhost", "foobar");
	    System.out.println("Isattrval cfhost=foobar : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.isattrval("cfhost", "hypei");
	    System.out.println("Isattrval cfhost=hypei : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    nodes = gh.query("mgmt");
	    for (String tmp : nodes)
		System.out.println("Query mgmt: " + tmp);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    nodes = gh.query("mgmt||login");
	    for (String tmp : nodes)
		System.out.println("Query mgmt||login: " + tmp);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    nodes = gh.query("mgmt&&login");
	    for (String tmp : nodes)
		System.out.println("Query mgmt&&login: " + tmp);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    nodes = gh.query("~mgmt");
	    for (String tmp : nodes)
		System.out.println("Query ~mgmt: " + tmp);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testquery("mgmt");
	    System.out.println("TestQuery mgmt: " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testquery("login");
	    System.out.println("TestQuery login: " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testquery("hype355", "mgmt");
	    System.out.println("TestQuery mgmt: " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testquery("hype355", "login");
	    System.out.println("TestQuery login: " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testquery("hype356", "mgmt");
	    System.out.println("TestQuery mgmt: " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testquery("hype356", "login");
	    System.out.println("TestQuery login: " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	String val;

	try {
	    val = gh.getattrval("mgmt");
	    System.out.println("Getattrval mgmt : " + val);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    val = gh.getattrval("login");
	    System.out.println("Getattrval login : " + val);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    val = gh.getattrval("cfhost");
	    System.out.println("Getattrval cfhost : " + val);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    val = gh.getattrval("hype355", "mgmt");
	    System.out.println("Getattrval hype355 mgmt : " + val);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    val = gh.getattrval("hype355", "login");
	    System.out.println("Getattrval hype355 login : " + val);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    val = gh.getattrval("hype355", "cfhost");
	    System.out.println("Getattrval hype355 cfhost : " + val);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    val = gh.getattrval("hype356", "mgmt");
	    System.out.println("Getattrval hype356 mgmt : " + val);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    val = gh.getattrval("hype356", "login");
	    System.out.println("Getattrval hype356 login : " + val);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    val = gh.getattrval("hype356", "cfhost");
	    System.out.println("Getattrval hype356 cfhost : " + val);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattr("mgmt");
	    System.out.println("Testattr mgmt : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattr("login");
	    System.out.println("Testattr login : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattr("cfhost");
	    System.out.println("Testattr cfhost : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattr("hype355", "mgmt");
	    System.out.println("Testattr hype355 mgmt : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattr("hype355", "login");
	    System.out.println("Testattr hype355 login : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattr("hype355", "cfhost");
	    System.out.println("Testattr hype355 cfhost : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattr("hype356", "mgmt");
	    System.out.println("Testattr hype356 mgmt : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattr("hype356", "login");
	    System.out.println("Testattr hype356 login : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattr("hype356", "cfhost");
	    System.out.println("Testattr hype356 cfhost : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("mgmt", "");
	    System.out.println("Testattrval mgmt= : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("mgmt", "foobar");
	    System.out.println("Testattrval mgmt=foobar : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("login", "");
	    System.out.println("Testattrval login= : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("login", "foobar");
	    System.out.println("Testattrval login=foobar : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("cfhost", "hypei");
	    System.out.println("Testattrval cfhost=hypei : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("cfhost", "foobar");
	    System.out.println("Testattrval cfhost=foobar : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("hype355", "mgmt", "");
	    System.out.println("Testattrval hype355 mgmt= : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("hype355", "mgmt", "foobar");
	    System.out.println("Testattrval hype355 mgmt=foobar : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("hype355", "login", "");
	    System.out.println("Testattrval hype355 login= : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("hype355", "login", "foobar");
	    System.out.println("Testattrval hype355 login=foobar : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("hype355", "cfhost", "hypei");
	    System.out.println("Testattrval hype355 cfhost=hypei : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("hype355", "cfhost", "foobar");
	    System.out.println("Testattrval hype355 cfhost=foobar : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("hype356", "mgmt", "");
	    System.out.println("Testattrval hype356 mgmt= : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("hype356", "mgmt", "foobar");
	    System.out.println("Testattrval hype356 mgmt=foobar : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("hype356", "login", "");
	    System.out.println("Testattrval hype356 login= : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("hype356", "login", "foobar");
	    System.out.println("Testattrval hype356 login=foobar : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("hype356", "cfhost", "hypei");
	    System.out.println("Testattrval hype356 cfhost=hypei : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    b = gh.testattrval("hype356", "cfhost", "foobar");
	    System.out.println("Testattrval hype356 cfhost=foobar : " + b);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	int errcount;
	try {
	    errcount = gh.parse();
	    System.out.println("Parse default: " + errcount);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    errcount = gh.parse("./testgenders");
	    System.out.println("Parse ./testgenders: " + errcount);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    errcount = gh.parse("./errtestgenders");
	    System.out.println("Parse ./errtestgenders: " + errcount);
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}

	try {
	    gh.getattr("foobarnode");
	}
	catch (GendersExceptionNotfound e) {
	    System.out.println("Got not found exception : " + e);
	}
	catch (GendersException e) {
	    System.out.println("Got exception : " + e);
	}

	gh.cleanup();

	try  {
	    System.out.println("NumNodes = " + gh.getnumnodes());
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}
   }

    public static void main(String[] args)
    {
	/* test (null); */
	test ("./testgenders");
    }
}