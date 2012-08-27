package gov.llnl.lc.chaos;

/**
 * Genders Parse Exception, indicates a parse error in the genders
 * database.
 */
public class GendersExceptionParse extends GendersException
{
    public GendersExceptionParse(String msg)
    {
	super(msg);
    }
}
