package gov.llnl.lc.chaos;

/**
 * Genders Notfound Exception, indicates a node, attribute, or other
 * input cannot be found.
 */
public class GendersExceptionNotfound extends GendersException
{
    public GendersExceptionNotfound(String msg)
    {
	super(msg);
    }
}
