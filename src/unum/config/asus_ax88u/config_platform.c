// (c) 2019-2020 minim.co
// unum device configuration management code

#include "unum.h"

/* Drop all the debug log messages from this file until needed */
#undef LOG_DBG_DST
#define LOG_DBG_DST LOG_DST_DROP


// Get device config
// Returns: pointer to the 0-terminated config string or NULL if fails,
//          the returned pointer has to be released with the
//          platform_cfg_free() call ASAP, if successful the returned config
//          length is stored in p_len (uness p_len is NULL). The length
//          includes the terminating 0.
char *platform_cfg_get(int *p_len)
{
	return NULL;
}

// Get config if it has changed since the last send
// (in)  last_sent_uid - UID of the last sent config
// (out) new_uid - ptr to UID of the new config
// (out) p_len - ptr where to store config length (or NULL)
// Returns: pointer to the 0-terminated config string
//          or NULL if unchanged, new_uid - is filled in
//          with the new UID if returned pointer is not NULL,
//          the returned pointer has to be released with the
//          platform_cfg_free() call ASAP. The length
//          includes the terminating 0.
char *platform_cfg_get_if_changed(CONFIG_UID_t *p_last_sent_uid,
                                  CONFIG_UID_t *p_new_uid, int *p_len)
{
    return NULL;
}

// Release the config mem returned by platform_cfg_get*()
void platform_cfg_free(char *buf)
{
    UTIL_FREE(buf);
    return;
}

// Apply configuration from the memory buffer
// Parameters:
// - pointer to the config memory buffer
// - config length
// Returns: 0 - if ok or error
// Note: cfg must be 0-terminated (done automaticaly
//       if data come from the HTTP response structure)
// Note: For this function reboots the device if
//       successful (only returns if failed)
int platform_apply_cloud_cfg(char *cfg, int cfg_len)
{
    return 0;
}

// Platform specific init for the config subsystem
int platform_cfg_init(void)
{
    return 0;
}
