/**
 * NanoCam Application Interface
 *
 * Copyright 2015 GomSpace ApS. All rights reserved.
 */

#ifndef __NANOCAM_H__
#define __NANOCAM_H__

#include <stdint.h>
#include <nanocam_types.h>

/** @brief Image list callback
 *
 * This callback is called once for each element in the image listing. If no
 * images are available, the function is called once with elm equal to NULL.
 *
 * @param seq Element sequence number, starting from 0.
 * @param elm Pointer to image list element structure
 */
typedef void (*nanocam_img_list_cb)(int seq, cam_list_element_t *elm);

/* @brief Set camera CSP address
 *
 * This function sets the CSP address of the camera for all other commands.
 *
 * @param node CSP address of camera
 */
void nanocam_set_node(uint8_t node);

/** @brief Capture a picture
 *
 * This function captures a picture with the camera.
 *
 * @return 0 on success, error code on error
 */
int nanocam_snap(cam_snap_t *snap, cam_snap_reply_t *reply, unsigned int timeout);

/** @brief Store a captured image
 *
 * This function stores captured image from the snap buffer to either flash or RAM.
 *
 * @return 0 on success, error code on error
 */
int nanocam_store(cam_store_t *store, cam_store_reply_t *reply, unsigned int timeout);

/** @brief Read sensor register
 *
 * @return 0 on success, error code on error
 */
int nanocam_reg_read(uint8_t reg, uint16_t *value, unsigned int timeout);

/** @brief Write sensor register
 *
 * @return 0 on success, error code on error
 */
int nanocam_reg_write(uint8_t reg, uint16_t value, unsigned int timeout);

/** @brief List RAM images
 *
 * @return 0 on success, error code on error
 */
int nanocam_img_list(nanocam_img_list_cb cb, unsigned int timeout);

/** @brief Flush RAM images
 *
 * @return 0 on success, error code on error
 */
int nanocam_img_list_flush(unsigned int timeout);

/** @brief Focus assist routine
 *
 * Run one iteration of focus assist routine.
 *
 * @return 0 on success, error code on error
 */
int nanocam_focus(uint8_t algorithm, uint32_t *af, unsigned int timeout);

/** @brief Recover data partition
 *
 * @return 0 on success, error code on error
 */
int nanocam_recoverfs(unsigned int timeout);

#endif /* __NANOCAM_H__ */
