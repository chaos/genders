package gov.llnl.lc.chaos;

/**
 * Genders Read Exception, indicates a read error on the genders
 * database
 */
public class GendersExceptionRead extends GendersException
{
    public GendersExceptionRead(String msg)
    {
	super(msg);
    }
}
