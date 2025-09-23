/**
 * NanoCam Client library
 *
 * Copyright 2015 GomSpace ApS. All rights reserved.
 */

#include <nanocam.h>

#include <inttypes.h>
#include <string.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <util/console.h>
#include <util/log.h>
#include <util/color_printf.h>

#include <csp/csp.h>
#include <csp/csp_endian.h>

static uint8_t node_cam = NANOCAM_DEFAULT_ADDRESS;

void nanocam_set_node(uint8_t node)
{
	node_cam = node;
}

int nanocam_snap(cam_snap_t *snap, cam_snap_reply_t *reply, unsigned int timeout)
{
	snap->flags  = csp_hton32(snap->flags);
	snap->delay  = csp_hton16(snap->delay);
	snap->width  = csp_hton16(snap->width);
	snap->height = csp_hton16(snap->height);
	snap->top    = csp_hton16(snap->top);
	snap->left   = csp_hton16(snap->left);

	int status = csp_transaction(CSP_PRIO_NORM, node_cam,
				     NANOCAM_PORT_SNAP, timeout,
				     snap, sizeof(*snap),
				     reply, sizeof(*reply));
	if (status != sizeof(*reply))
		return -1;

	return reply->result;
}

int nanocam_store(cam_store_t *store, cam_store_reply_t *reply, unsigned int timeout)
{
	store->flags = csp_hton32(store->flags);
	store->filename[sizeof(store->filename) - 1] = '\0';
	store->reserved[0] = 0; /* Ensure reserved bytes are 0 */
	store->reserved[1] = 0;

	unsigned int status = csp_transaction(CSP_PRIO_NORM, node_cam,
					      NANOCAM_PORT_STORE, timeout,
					      store, sizeof(*store),
					      reply, sizeof(*reply));
	if (status != sizeof(*reply))
		return status;

	reply->image_ptr = csp_ntoh32(reply->image_ptr);
	reply->image_size = csp_ntoh32(reply->image_size);

	return 0;
}

int nanocam_reg_read(uint8_t reg, uint16_t *value, unsigned int timeout)
{
	cam_reg_t request, reply;
	request.reg_addr = reg;

	int status = csp_transaction(CSP_PRIO_HIGH, node_cam,
				     NANOCAM_PORT_REG_READ, timeout,
				     &request, sizeof(request),
				     &reply, sizeof(reply));
	if (status < 0)
		return -2;

	*value = csp_ntoh16(reply.reg_value);

	return 0;
}

int nanocam_reg_write(uint8_t reg, uint16_t value, unsigned int timeout)
{
	cam_reg_t request, reply;
	request.reg_addr = reg;
	request.reg_value = csp_hton16(value);

	int status = csp_transaction(CSP_PRIO_HIGH, node_cam,
				     NANOCAM_PORT_REG_WRITE, timeout,
				     &request, sizeof(request),
				     &reply, sizeof(reply));
	if (status < 0)
		return -2;

	return reply.result;
}

int nanocam_img_list(nanocam_img_list_cb cb, unsigned int timeout)
{
	int i;
	csp_conn_t *conn;
	csp_packet_t *packet;
	cam_list_t *list;
	cam_list_reply_t *reply;
	cam_list_element_t *elm;

	packet = csp_buffer_get(sizeof(*list));
	if (!packet)
		return -1;

	list = (cam_list_t *) packet->data;
	list->elements = 3;
	list->reserved[0] = 0; /* Ensure reserved bytes are 0 */
	list->reserved[1] = 0;
	list->reserved[2] = 0;
	list->flags = 0;

	conn = csp_connect(CSP_PRIO_NORM, node_cam, NANOCAM_PORT_IMG_LIST,
			   timeout, CSP_O_NONE);
	if (!conn) {
		csp_buffer_free(packet);
		return -1;
	}

	packet->length = sizeof(*list);
	if (!csp_send(conn, packet, 0))
		csp_buffer_free(packet);

	while ((packet = csp_read(conn, timeout))) {
		reply = (cam_list_reply_t *) packet->data;

		reply->total = csp_ntoh16(reply->total);
		reply->seq = csp_ntoh16(reply->seq);

		if (reply->total == 0) {
			cb(0, NULL);
			csp_buffer_free(packet);
			break;
		}

		for (i = 0; i < reply->count ; i++) {
			elm = &reply->images[i];
			elm->flags = csp_ntoh16(elm->flags);
			elm->width = csp_ntoh16(elm->width);
			elm->height = csp_ntoh16(elm->height);
			elm->ptr = csp_ntoh32(elm->ptr);
			elm->size = csp_ntoh32(elm->size);
			elm->time = csp_ntoh64(elm->time);

			/* Callback */
			cb(reply->seq + i, elm);
		}

		if (reply->seq + reply->count == reply->total) {
			csp_buffer_free(packet);
			break;
		}

		csp_buffer_free(packet);
	}

	csp_close(conn);

	return 0;
}


int nanocam_img_list_flush(unsigned int timeout)
{
	cam_list_flush_t request;
	cam_list_flush_reply_t reply;

	memset(&request, 0, sizeof(request));

	request.flags = csp_hton32(0);
	request.magic = csp_hton32(NANOCAM_IMG_FLUSH_MAGIC);

	unsigned int status = csp_transaction(CSP_PRIO_NORM,
				     node_cam, NANOCAM_PORT_IMG_FLUSH,
				     timeout,
				     &request, sizeof(request),
				     &reply, sizeof(reply));
	if (status < sizeof(reply))
		return status;

	return reply.result;
}

int nanocam_focus(uint8_t algorithm, uint32_t *focus, unsigned int timeout)
{
	cam_focus_t request;
	cam_focus_reply_t reply;

	request.algorithm = algorithm;
	request.reserved[0] = 0; /* Ensure reserved bytes are 0 */
	request.reserved[1] = 0;
	request.reserved[2] = 0;

	unsigned int status = csp_transaction(CSP_PRIO_NORM,
				     node_cam, NANOCAM_PORT_FOCUS,
				     timeout,
				     &request, sizeof(request),
				     &reply, sizeof(reply));
	if (status < sizeof(reply))
		return status;

	*focus = csp_ntoh32(reply.focus);

	return 0;
}

int nanocam_recoverfs(unsigned int timeout)
{
	cam_recoverfs_t request;
	cam_recoverfs_reply_t reply;

	request.flags = csp_hton32(0);
	request.magic = csp_hton32(NANOCAM_RECOVERFS_MAGIC);

	unsigned int status = csp_transaction(CSP_PRIO_NORM,
				     node_cam, NANOCAM_PORT_RECOVERFS,
				     timeout,
				     &request, sizeof(request),
				     &reply, sizeof(reply));
	if (status < sizeof(reply))
		return status;

	return reply.result;
}
