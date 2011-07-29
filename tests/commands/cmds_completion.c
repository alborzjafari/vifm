#include <stdlib.h>
#include <string.h>

#include "seatest.h"

#include "../../src/cmds.h"
#include "../../src/completion.h"
#include "../../src/macros.h"

static struct cmd_info cmdi;

static int goto_cmd(const struct cmd_info* cmd_info);
static int delete_cmd(const struct cmd_info* cmd_info);

static const struct cmd_add commands[] = {
	{ .name = "",       .abbr = NULL, .handler = goto_cmd,   .id = -1,    .range = 1,    .cust_sep = 0,
		.emark = 0,       .qmark = 0,   .expand = 0,           .regexp = 0, .min_args = 0, .max_args = 0, },
	{ .name = "delete", .abbr = "d",  .handler = delete_cmd, .id = 1,     .range = 1,    .cust_sep = 0,
		.emark = 1,       .qmark = 0,   .expand = 0,           .regexp = 0, .min_args = 0, .max_args = 1, },
};

static int
goto_cmd(const struct cmd_info* cmd_info)
{
	return 0;
}

static int
delete_cmd(const struct cmd_info* cmd_info)
{
	cmdi = *cmd_info;
	return 0;
}

static void
setup(void)
{
	add_buildin_commands(commands, ARRAY_LEN(commands));
}

static void
teardown(void)
{
}

static void
test_skip_goto(void)
{
	char *completion;

	reset_completion();
	assert_int_equal(0, complete_cmd(""));
	assert_int_equal(4, get_completion_count());
	if(get_completion_count() == 0)
		return;
	completion = next_completion();
	assert_string_equal("command", completion);
	free(completion);
	completion = next_completion();
	assert_string_equal("delcommand", completion);
	free(completion);
	completion = next_completion();
	assert_string_equal("delete", completion);
	free(completion);
}

static void
test_skip_abbreviations(void)
{
	char *completion;

	reset_completion();
	assert_int_equal(0, complete_cmd("d"));
	assert_int_equal(3, get_completion_count());
	if(get_completion_count() == 0)
		return;
	completion = next_completion();
	assert_string_equal("delcommand", completion);
	free(completion);
	completion = next_completion();
	assert_string_equal("delete", completion);
	free(completion);
}

static void
test_dont_remove_range(void)
{
	char *completion;

	reset_completion();
	assert_int_equal(2, complete_cmd("% del"));
	assert_int_equal(3, get_completion_count());
	if(get_completion_count() == 0)
		return;
	completion = next_completion();
	assert_string_equal("delcommand", completion);
	free(completion);

	reset_completion();
	assert_int_equal(1, complete_cmd("3del"));
	assert_int_equal(3, get_completion_count());
	if(get_completion_count() == 0)
		return;
	completion = next_completion();
	assert_string_equal("delcommand", completion);
	free(completion);
}

static void
test_dont_remove_cmd(void)
{
	char *completion;

	reset_completion();
	assert_int_equal(7, complete_cmd("% dele "));
	assert_int_equal(3, get_completion_count());
	if(get_completion_count() == 0)
		return;
	completion = next_completion();
	assert_string_equal("fastrun", completion);
	free(completion);
	completion = next_completion();
	assert_string_equal("followlinks", completion);
	free(completion);
}

static void
test_skip_complete_args(void)
{
	char *completion;

	reset_completion();
	assert_int_equal(17, complete_cmd("% dele fast slow "));
	assert_int_equal(3, get_completion_count());
	if(get_completion_count() == 0)
		return;
	completion = next_completion();
	assert_string_equal("fastrun", completion);
	free(completion);
	completion = next_completion();
	assert_string_equal("followlinks", completion);
	free(completion);
}

static void
test_com_completion(void)
{
	char *completion;

	assert_int_equal(0, execute_cmd("command udf a"));

	reset_completion();
	assert_int_equal(4, complete_cmd("com u"));
	assert_int_equal(2, get_completion_count());
	if(get_completion_count() == 0)
		return;
	completion = next_completion();
	assert_string_equal("udf", completion);
	free(completion);
}

static void
test_delc_completion(void)
{
	char *completion;

	assert_int_equal(0, execute_cmd("command udf a"));

	reset_completion();
	assert_int_equal(5, complete_cmd("delc u"));
	assert_int_equal(2, get_completion_count());
	if(get_completion_count() == 0)
		return;
	completion = next_completion();
	assert_string_equal("udf", completion);
	free(completion);
}

void
completion_tests(void)
{
	test_fixture_start();

	fixture_setup(setup);
	fixture_teardown(teardown);

	run_test(test_skip_goto);
	run_test(test_skip_abbreviations);
	run_test(test_dont_remove_range);
	run_test(test_dont_remove_cmd);
	run_test(test_skip_complete_args);
	run_test(test_com_completion);
	run_test(test_delc_completion);

	test_fixture_end();
}

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab : */
