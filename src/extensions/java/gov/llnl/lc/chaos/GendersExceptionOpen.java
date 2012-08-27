package gov.llnl.lc.chaos;

/**
 * Genders Open Exception, indicates a genders database cannot be
 * open, possiblity due to permissions or an incorrect filename.
 */
public class GendersExceptionOpen extends GendersException
{
    public GendersExceptionOpen(String msg)
    {
	super(msg);
    }
}
