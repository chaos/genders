package gov.llnl.lc.chaos;

/**
 * Genders Syntax Exception, indicates a syntax error in a genders
 * query.
 */
public class GendersExceptionSyntax extends GendersException
{
    public GendersExceptionSyntax(String msg)
    {
	super(msg);
    }
}
