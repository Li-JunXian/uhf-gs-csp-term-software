/**
 * @file getopt.h
 *
 * This implements optional argument parsing
 *
 * @author Jeppe Ledet-Pedersen
 * Copyright 2015 GomSpace ApS. All rights reserved.
 */

#ifndef _GOSH_GETOPT_H_
#define _GOSH_GETOPT_H_

#include <command/command.h>

int gosh_getopt(struct command_context *ctx, const char *opts);

#endif /* _GOSH_GETOPT_H_ */
