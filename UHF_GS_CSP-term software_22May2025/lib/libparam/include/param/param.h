/**
 * GomSpace Parameter System
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef PARAM_H_
#define PARAM_H_

#include <conf_param.h>

#if ENABLE_PARAM_HOST
#include <param_host.h>
#endif

#if ENABLE_PARAM_CLIENT
#include <param/rparam_client.h>
#endif

#if ENABLE_PARAM_SERVER
#include <rparam_server.h>
#endif

#include <param/param_serializer.h>
#include <param/param_fletcher.h>
#include <param/param_string.h>
#include <param/param_types.h>

#endif /* PARAM_H_ */
