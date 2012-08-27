package gov.llnl.lc.chaos;

/**
 * Genders Internal Exception, all other errors that may occur due to
 * system issues.
 */
public class GendersExceptionInternal extends GendersException
{
    public GendersExceptionInternal(String msg)
    {
	super(msg);
    }
}
