#include <criterion/criterion.h>
#include "../src/lib/appstate.h"

Test(app_state_tests, app_state_convert_returns_error)
{
	AppState app = {0};
	app.convert = "1.1.1.1";
	app.format = "decimal";

	cr_assert(ERR_ARG_INVALID == app_state_convert(&app));
}