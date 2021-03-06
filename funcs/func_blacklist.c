/*
 * Asterisk -- An open source telephony toolkit.
 *
 * Copyright (C) 1999 - 2005, Digium, Inc.
 *
 * Mark Spencer <markster@digium.com>
 *
 * See http://www.iaxproxy.org for more information about
 * the Asterisk project. Please do not directly contact
 * any of the maintainers of this project for assistance;
 * the project provides a web site, mailing lists and IRC
 * channels for your use.
 *
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 */

/*! \file
 *
 * \brief Function to lookup the callerid number, and see if it is blacklisted
 *
 * \author Mark Spencer <markster@digium.com>
 *
 * \ingroup functions
 * 
 */

#include "iaxproxy.h"

ASTERISK_FILE_VERSION(__FILE__, "$Revision: 154542 $")

#include "iaxproxy/pbx.h"
#include "iaxproxy/module.h"
#include "iaxproxy/channel.h"
#include "iaxproxy/astdb.h"

/*** DOCUMENTATION
	<function name="BLACKLIST" language="en_US">
		<synopsis>
			Check if the callerid is on the blacklist.
		</synopsis>
		<syntax />
		<description>
			<para>Uses astdb to check if the Caller*ID is in family <literal>blacklist</literal>.
			Returns <literal>1</literal> or <literal>0</literal>.</para>
		</description>
		<see-also>
			<ref type="function">DB</ref>
		</see-also>
	</function>

***/

static int blacklist_read(struct ast_channel *chan, const char *cmd, char *data, char *buf, size_t len)
{
	char blacklist[1];
	int bl = 0;

	if (chan->cid.cid_num) {
		if (!ast_db_get("blacklist", chan->cid.cid_num, blacklist, sizeof (blacklist)))
			bl = 1;
	}
	if (chan->cid.cid_name) {
		if (!ast_db_get("blacklist", chan->cid.cid_name, blacklist, sizeof (blacklist)))
			bl = 1;
	}

	snprintf(buf, len, "%d", bl);
	return 0;
}

static struct ast_custom_function blacklist_function = {
	.name = "BLACKLIST",
	.read = blacklist_read,
};

static int unload_module(void)
{
	return ast_custom_function_unregister(&blacklist_function);
}

static int load_module(void)
{
	return ast_custom_function_register(&blacklist_function);
}

AST_MODULE_INFO_STANDARD(ASTERISK_GPL_KEY, "Look up Caller*ID name/number from blacklist database");
