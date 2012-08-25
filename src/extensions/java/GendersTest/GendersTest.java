import gov.llnl.lc.chaos.Genders;
import gov.llnl.lc.chaos.GendersException;
import gov.llnl.lc.chaos.GendersExceptionNotfound;

class GendersTest
{
    public static void main(String[] args)
    {
	Genders gh = new Genders("GendersTest/testgenders");

	try {
	    System.out.println("NumNodes = " + gh.getnumnodes());
	    System.out.println("Numattrs = " + gh.getnumattrs());
	    System.out.println("Maxattrs = " + gh.getmaxattrs());
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
	}
	catch (Exception e) {
	    System.out.println("exception: " + e);
	}

	String attrs[];
	String vals[];

	try {
	    attrs = gh.getattr();
	    for (String tmp : attrs)
		System.out.println("getattr : " + tmp); 

	    attrs = gh.getattr("hype355");
	    for (String tmp : attrs)
		System.out.println("getattr hype355: " + tmp); 

	    attrs = gh.getattr_all();
	    for (String tmp : attrs)
		System.out.println("Getattr All : " + tmp);
	}
	catch (Exception e) {
	    System.out.println("exception: " + e);
	}

	boolean b;

	try {
	    b = gh.isnode("foobar");
	    System.out.println("Isnode foobar : " + b);

	    b = gh.isnode("hype355");
	    System.out.println("Isnode hype355 : " + b);

	    b = gh.isnode(null);
	    System.out.println("Isnode null : " + b);

	    b = gh.isattr("foobar");
	    System.out.println("Isattr foobar : " + b);

	    b = gh.isattr("mgmt");
	    System.out.println("Isattr mgmt : " + b);

	    b = gh.isattrval("cfhost", "foobar");
	    System.out.println("Isattrval cfhost=foobar : " + b);

	    b = gh.isattrval("cfhost", "hypei");
	    System.out.println("Isattrval cfhost=hypei : " + b);
	}
	catch (Exception e) {
	    System.out.println("exception: " + e);
	}

	try {
	    nodes = gh.query("mgmt");
	    for (String tmp : nodes)
		System.out.println("Query mgmt: " + tmp);

	    nodes = gh.query("mgmt||login");
	    for (String tmp : nodes)
		System.out.println("Query mgmt||login: " + tmp);

	    nodes = gh.query("mgmt&&login");
	    for (String tmp : nodes)
		System.out.println("Query mgmt&&login: " + tmp);

	    nodes = gh.query("~mgmt");
	    for (String tmp : nodes)
		System.out.println("Query ~mgmt: " + tmp);

	    b = gh.testquery("mgmt");
	    System.out.println("TestQuery mgmt: " + b);

	    b = gh.testquery("login");
	    System.out.println("TestQuery login: " + b);

	    b = gh.testquery("hype355", "mgmt");
	    System.out.println("TestQuery mgmt: " + b);

	    b = gh.testquery("hype355", "login");
	    System.out.println("TestQuery login: " + b);

	    b = gh.testquery("hype356", "mgmt");
	    System.out.println("TestQuery mgmt: " + b);

	    b = gh.testquery("hype356", "login");
	    System.out.println("TestQuery login: " + b);
	}
	catch (Exception e) {
	    System.out.println("exception: " + e);
	}

	String val;

	try {
	    val = gh.getattrval("mgmt");
	    System.out.println("Getattrval mgmt : " + val);

	    val = gh.getattrval("login");
	    System.out.println("Getattrval login : " + val);

	    val = gh.getattrval("cfhost");
	    System.out.println("Getattrval cfhost : " + val);

	    val = gh.getattrval("hype355", "mgmt");
	    System.out.println("Getattrval hype355 mgmt : " + val);

	    val = gh.getattrval("hype355", "login");
	    System.out.println("Getattrval hype355 login : " + val);
	}
	catch (Exception e) {
	    System.out.println("exception: " + e);
	}

	try {
	    val = gh.getattrval("hype355", "cfhost");
	    System.out.println("Getattrval hype355 cfhost : " + val);

	    val = gh.getattrval("hype356", "mgmt");
	    System.out.println("Getattrval hype356 mgmt : " + val);

	    val = gh.getattrval("hype356", "login");
	    System.out.println("Getattrval hype356 login : " + val);

	    val = gh.getattrval("hype356", "cfhost");
	    System.out.println("Getattrval hype356 cfhost : " + val);

	    b = gh.testattr("mgmt");
	    System.out.println("Testattr mgmt : " + b);

	    b = gh.testattr("login");
	    System.out.println("Testattr login : " + b);

	    b = gh.testattr("cfhost");
	    System.out.println("Testattr cfhost : " + b);

	    b = gh.testattr("hype355", "mgmt");
	    System.out.println("Testattr hype355 mgmt : " + b);

	    b = gh.testattr("hype355", "login");
	    System.out.println("Testattr hype355 login : " + b);

	    b = gh.testattr("hype355", "cfhost");
	    System.out.println("Testattr hype355 cfhost : " + b);

	    b = gh.testattr("hype356", "mgmt");
	    System.out.println("Testattr hype356 mgmt : " + b);

	    b = gh.testattr("hype356", "login");
	    System.out.println("Testattr hype356 login : " + b);

	    b = gh.testattr("hype356", "cfhost");
	    System.out.println("Testattr hype356 cfhost : " + b);

	    b = gh.testattrval("mgmt", "");
	    System.out.println("Testattrval mgmt= : " + b);

	    b = gh.testattrval("mgmt", "foobar");
	    System.out.println("Testattrval mgmt=foobar : " + b);

	    b = gh.testattrval("login", "");
	    System.out.println("Testattrval login= : " + b);

	    b = gh.testattrval("login", "foobar");
	    System.out.println("Testattrval login=foobar : " + b);

	    b = gh.testattrval("cfhost", "hypei");
	    System.out.println("Testattrval cfhost=hypei : " + b);

	    b = gh.testattrval("cfhost", "foobar");
	    System.out.println("Testattrval cfhost=foobar : " + b);

	    b = gh.testattrval("hype355", "mgmt", "");
	    System.out.println("Testattrval hype355 mgmt= : " + b);

	    b = gh.testattrval("hype355", "mgmt", "foobar");
	    System.out.println("Testattrval hype355 mgmt=foobar : " + b);

	    b = gh.testattrval("hype355", "login", "");
	    System.out.println("Testattrval hype355 login= : " + b);

	    b = gh.testattrval("hype355", "login", "foobar");
	    System.out.println("Testattrval hype355 login=foobar : " + b);

	    b = gh.testattrval("hype355", "cfhost", "hypei");
	    System.out.println("Testattrval hype355 cfhost=hypei : " + b);

	    b = gh.testattrval("hype355", "cfhost", "foobar");
	    System.out.println("Testattrval hype355 cfhost=foobar : " + b);

	    b = gh.testattrval("hype356", "mgmt", "");
	    System.out.println("Testattrval hype356 mgmt= : " + b);

	    b = gh.testattrval("hype356", "mgmt", "foobar");
	    System.out.println("Testattrval hype356 mgmt=foobar : " + b);

	    b = gh.testattrval("hype356", "login", "");
	    System.out.println("Testattrval hype356 login= : " + b);

	    b = gh.testattrval("hype356", "login", "foobar");
	    System.out.println("Testattrval hype356 login=foobar : " + b);

	    b = gh.testattrval("hype356", "cfhost", "hypei");
	    System.out.println("Testattrval hype356 cfhost=hypei : " + b);

	    b = gh.testattrval("hype356", "cfhost", "foobar");
	    System.out.println("Testattrval hype356 cfhost=foobar : " + b);
	}
	catch (Exception e) {
	    System.out.println("exception: " + e);
	}

	int errcount;

	try {
	    errcount = gh.parse();
	    System.out.println("Parse default: " + errcount);

	    errcount = gh.parse("GendersTest/testgenders");
	    System.out.println("Parse GendersTest/testgenders: " + errcount);

	    errcount = gh.parse("GendersTest/errtestgenders");
	    System.out.println("Parse GendersTest/errtestgenders: " + errcount);
	}
	catch (Exception e) {
	    System.out.println("exception: " + e);
	}

	try {
	    gh.getattr("foobarnode");
	}
	catch (GendersExceptionNotfound e) {
	    System.out.println("Got not found exception : " + e);
	}
	catch (GendersException e) {
	    System.out.println("Got genders exception : " + e);
	}
	catch (Exception e) {
	    System.out.println("exception: " + e);
	}
	finally {
	    System.out.println("uncaught exception");
	}

	gh.cleanup();

	try  {
	    System.out.println("NumNodes = " + gh.getnumnodes());
	}
	catch (Exception e) {
	    System.out.println("getattr hype355: " + e);
	}
    }
}