#include <stic.h>

#include "../../src/filetype.h"

DEFINE_SUITE();

SETUP()
{
	ft_init(NULL);
}

TEARDOWN()
{
	ft_reset(0);
}

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab cinoptions-=(0 : */
/* vim: set cinoptions+=t0 : */
