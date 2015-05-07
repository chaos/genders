
#include <genders.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

static int genders_file_exists (const char *genders_file)
{
	struct stat st;

	if (genders_file == NULL)
		genders_file = GENDERS_DEFAULT_FILE;

	if ((stat (genders_file, &st) < 0) && 
		((errno == ENOENT) || (errno == ENOTDIR)))
		return 0;

	return 1;
}


/*
 *  This is a helper symbol exported by cfengine to allow
 *   modules to export new hard classes.
 */
extern CfRegisterHardClass (const char *class);

int cfagent_module_getclasses (void)
{
	genders_t gh;
	char **attrs, **vals;
	int i, nattrs;
	int maxattrs;
	int rc = -1;

	char *host = getenv ("GENDERS_HOSTNAME");
	char *genders_file = getenv ("GENDERS_FILE");

	/*
	 *  Return silently if genders file does not exist. This
	 *   shouldn't be an error, there is just nothing to do.
	 */
	if (!genders_file_exists (genders_file))
		return (0);

	if ((gh = genders_handle_create()) == NULL)
		return (-1);

	if (genders_load_data (gh, genders_file) < 0) {
		fprintf (stderr, "genders: Failed to read genders file: %s\n",
				genders_errormsg (gh));
		return (-1);
	}

	if (genders_attrlist_create (gh, &attrs) < 0) {
		fprintf (stderr, "genders: Failed to malloc attrlist: %s\n",
				genders_errormsg (gh));
		goto out1;
	}
	if (genders_vallist_create (gh, &vals) < 0) {
		fprintf (stderr, "genders: Failed to malloc vallist: %s\n",
				genders_errormsg (gh));
		goto out2;
	}

	maxattrs = genders_getmaxattrs (gh);

	if ((nattrs = genders_getattr (gh, attrs, vals, maxattrs, host)) < 0) {
		fprintf (stderr, "genders: Failed to get all node attributes: %s\n",
				genders_errormsg (gh));
		goto out3;
	}

	rc = 0;

	for (i = 0; i < nattrs; i++) {
		CfRegisterHardClass (attrs[i]);
		if (vals[i][0] != '\0') {
			char buf [8192];
			snprintf (buf, sizeof (buf) - 1, "%s_is_%s", attrs[i], vals[i]);
			CfRegisterHardClass (buf);
		}
	}

out3:
	genders_attrlist_destroy (gh, attrs);
out2:
	genders_vallist_destroy (gh, vals);
out1:
	genders_handle_destroy (gh);
	return (rc);
}
