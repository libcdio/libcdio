/* Common Multimedia Command (MMC) routines.
  Copyright (C) 2004-2008, 2010-2012, 2014, 2025-2026
  Rocky Bernstein <rocky@gnu.org>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif

#include "cdio_private.h"
#include "cdtext_private.h"
#include <cdio/cdio.h>
#include <cdio/logging.h>
#include <cdio/mmc.h>
#include <cdio/mmc_cmds.h>
#include <cdio/util.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#include <assert.h>

const char *mmc_cmd2str(uint8_t command) {
  switch (command) {
  case CDIO_MMC_GPCMD_TEST_UNIT_READY:
    return "TEST UNIT READY";

  case CDIO_MMC_GPCMD_REQUEST_SENSE:
    return "REQUEST SENSE";

  case CDIO_MMC_GPCMD_FORMAT_UNIT:
    return "FORMAT UNIT";

  case CDIO_MMC_GPCMD_INQUIRY:
    return "INQUIRY";

  case CDIO_MMC_GPCMD_MODE_SELECT_6:
    return "MODE SELECT (6)";

  case CDIO_MMC_GPCMD_MODE_SENSE_6:
    return "MODE SENSE (6)";

  case CDIO_MMC_GPCMD_START_STOP_UNIT:
    return "START STOP UNIT";

  case CDIO_MMC_GPCMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
    return "PREVENT ALLOW MEDIUM REMOVAL";

  case CDIO_MMC_GPCMD_READ_FORMAT_CAPACITIES:
    return "READ FORMAT CAPACITIES";

  case CDIO_MMC_GPCMD_READ_CAPACITIY:
    return "READ_CAPACITIY";

  case CDIO_MMC_GPCMD_READ_10:
    return "READ (10)";

  case CDIO_MMC_GPCMD_WRITE_10:
    return "WRITE (10)";

  case CDIO_MMC_GPCMD_SEEK_10:
    return "SEEK (10)";

  case CDIO_MMC_GPCMD_ERASE_10:
    return "ERASE (10)";

  case CDIO_MMC_GPCMD_WRITE_AND_VERIFY_10:
    return "WRITE AND VERIFY (10)";

  case CDIO_MMC_GPCMD_VERIFY_10:
    return "VERIFY (10)";

  case CDIO_MMC_GPCMD_SYNCHRONIZE_CACHE:
    return "SYNCHRONIZE CACHE";

  case CDIO_MMC_GPCMD_WRITE_BUFFER:
    return "WRITE BUFFER";

  case CDIO_MMC_GPCMD_READ_BUFFER:
    return "READ_BUFFER";

  case CDIO_MMC_GPCMD_READ_SUBCHANNEL:
    return "READ_SUBCHANNEL";

  case CDIO_MMC_GPCMD_READ_TOC:
    return "READ TOC";

  case CDIO_MMC_GPCMD_READ_HEADER:
    return "READ_HEADER";

  case CDIO_MMC_GPCMD_PLAY_AUDIO_10:
    return "PLAY AUDIO (10)";

  case CDIO_MMC_GPCMD_GET_CONFIGURATION:
    return "GET_CONFIGURATION";

  case CDIO_MMC_GPCMD_PLAY_AUDIO_MSF:
    return "PLAY AUDIO MSF";

  case CDIO_MMC_GPCMD_PLAY_AUDIO_TI:
    return "PLAY_AUDIO TI";

  case CDIO_MMC_GPCMD_PLAY_TRACK_REL_10:
    return "PLAY TRACK REL (10)";

  case CDIO_MMC_GPCMD_GET_EVENT_STATUS:
    return "GET EVENT STATUS";

  case CDIO_MMC_GPCMD_PAUSE_RESUME:
    return "PAUSE RESUME";

  case CDIO_MMC_GPCMD_READ_DISC_INFORMATION:
    return "READ DISC INFORMATION";

  case CDIO_MMC_GPCMD_READ_TRACK_INFORMATION:
    return "READ TRACK INFORMATION";

  case CDIO_MMC_GPCMD_RESERVE_TRACK:
    return "RESERVE TRACK";

  case CDIO_MMC_GPCMD_SEND_OPC_INFORMATION:
    return "SEND OPC INFORMATION";

  case CDIO_MMC_GPCMD_MODE_SELECT_10:
    return "MODE SELECT (10)";

  case CDIO_MMC_GPCMD_REPAIR_TRACK:
    return "REPAIR_TRACK";

  case CDIO_MMC_GPCMD_MODE_SENSE_10:
    return "MODE SENSE (10)";

  case CDIO_MMC_GPCMD_CLOSE_TRACK_SESSION:
    return "CLOSE TRACK SESSION";

  case CDIO_MMC_GPCMD_READ_BUFFER_CAPACITY:
    return "READ_BUFFER CAPACITY";

  case CDIO_MMC_GPCMD_SEND_CUE_SHEET:
    return "SEND_CUE SHEET";

  case CDIO_MMC_GPCMD_REPORT_LUNS:
    return "REPORT LUNS";

  case CDIO_MMC_GPCMD_BLANK:
    return "BLANK";

  case CDIO_MMC_GPCMD_SECURITY_PROTOCOL_IN:
    return "SECURITY PROTOCOL IN";

  case CDIO_MMC_GPCMD_SEND_KEY:
    return "SEND KEY";

  case CDIO_MMC_GPCMD_REPORT_KEY:
    return "REPORT KEY";

  case CDIO_MMC_GPCMD_PLAY_AUDIO_12:
    return "PLAY_AUDIO (12)";

  case CDIO_MMC_GPCMD_LOAD_UNLOAD:
    return "LOAD UNLOAD";

  case CDIO_MMC_GPCMD_SET_READ_AHEAD:
    return "SET READ AHEAD";

  case CDIO_MMC_GPCMD_READ_12:
    return "READ (12)";

  case CDIO_MMC_GPCMD_PLAY_TRACK_REL_12:
    return "PLAY_TRACK REL (12)";

  case CDIO_MMC_GPCMD_WRITE_12:
    return "WRITE (12)";

  case CDIO_MMC_GPCMD_READ_MEDIA_SERIAL_12:
    return "READ MEDIA SERIAL (12)";

  case CDIO_MMC_GPCMD_GET_PERFORMANCE:
    return "GET PERFORMANCE";

  case CDIO_MMC_GPCMD_READ_DVD_STRUCTURE:
    return "READ DVD STRUCTURE";

  case CDIO_MMC_GPCMD_SECURITY_PROTOCOL_OUT:
    return "SECURITY PROTOCOL_OUT";

  case CDIO_MMC_GPCMD_SET_STREAMING:
    return "SET STREAMING";

  case CDIO_MMC_GPCMD_READ_MSF:
    return "READ MSF";

  case CDIO_MMC_GPCMD_SET_SPEED:
    return "SET SPEED";

  case CDIO_MMC_GPCMD_MECHANISM_STATUS:
    return "MECHANISM STATUS";

  case CDIO_MMC_GPCMD_READ_CD:
    return "READ CD";

  case CDIO_MMC_GPCMD_SEND_DISC_STRUCTURE:
    return "SEND DISC STRUCTURE";

  case CDIO_MMC_GPCMD_CD_PLAYBACK_STATUS:
    return "CD PLAYBACK STATUS";

  case CDIO_MMC_GPCMD_PLAYBACK_CONTROL:
    return "PLAYBACK CONTROL";

  case CDIO_MMC_GPCMD_READ_CDDA:
    return "READ CDDA";

  case CDIO_MMC_GPCMD_READ_CDXA:
    return "READ CDXA";

  case CDIO_MMC_GPCMD_READ_ALL_SUBCODES:
    return "READ ALL SUBCODES";

  default: {
    char buf[30];
    snprintf(buf, sizeof(buf), "Unknown 0x%x", command);
    return strdup(buf);
  }
  }
}

/*************************************************************************
  MMC CdIo Operations which a driver may use.
  These are not accessible directly.

  Most of these routines just pick out the cdio pointer and call the
  corresponding publically-accessible routine.
*************************************************************************/

/**
  Read Audio Subchannel information

  @param p_user_data the CD object to be acted upon.

*/
driver_return_code_t
audio_read_subchannel_mmc(void *p_user_data, cdio_subchannel_t *p_subchannel) {
  generic_img_private_t *p_env = p_user_data;
  if (!p_env)
    return DRIVER_OP_UNINIT;
  return mmc_audio_read_subchannel(p_env->cdio, p_subchannel);
}

/**
  Get the block size for subsequent read requests, via MMC.
  @return the blocksize if > 0; error if <= 0
 */
int get_blocksize_mmc(void *p_user_data) {
  generic_img_private_t *p_env = p_user_data;
  if (!p_env)
    return DRIVER_OP_UNINIT;
  return mmc_get_blocksize(p_env->cdio);
}

/**
  Get the lsn of the end of the CD (via MMC).
*/
lsn_t get_disc_last_lsn_mmc(void *p_user_data) {
  generic_img_private_t *p_env = p_user_data;
  if (!p_env)
    return CDIO_INVALID_LSN;
  return mmc_get_disc_last_lsn(p_env->cdio);
}

void get_drive_cap_mmc(const void *p_user_data,
                       /*out*/ cdio_drive_read_cap_t *p_read_cap,
                       /*out*/ cdio_drive_write_cap_t *p_write_cap,
                       /*out*/ cdio_drive_misc_cap_t *p_misc_cap) {
  const generic_img_private_t *p_env = p_user_data;
  mmc_get_drive_cap(p_env->cdio, p_read_cap, p_write_cap, p_misc_cap);
}

/**
    Find out if media has changed since the last call.  @param
    p_user_data the environment of the CD object to be acted upon.
    @return 1 if media has changed since last call, 0 if not. Error
    return codes are the same as driver_return_code_t
*/
int get_media_changed_mmc(const void *p_user_data) {
  const generic_img_private_t *p_env = p_user_data;
  return mmc_get_media_changed(p_env->cdio);
}

char *get_mcn_mmc(const void *p_user_data) {
  const generic_img_private_t *p_env = p_user_data;
  return mmc_get_mcn(p_env->cdio);
}

driver_return_code_t get_tray_status(const void *p_user_data) {
  const generic_img_private_t *p_env = p_user_data;
  return mmc_get_tray_status(p_env->cdio);
}

/**
    Read sectors using SCSI-MMC GPCMD_READ_CD.
 */
driver_return_code_t mmc_read_data_sectors(CdIo_t *p_cdio, void *p_buf,
                                           lsn_t i_lsn, uint16_t i_blocksize,
                                           uint32_t i_blocks) {
  return mmc_read_cd(p_cdio, p_buf, /* place to store data */
                     i_lsn,         /* lsn */
                     0,             /* read_sector_type */
                     false,         /* digital audio play */
                     false,         /* return sync header */
                     0,             /* header codes */
                     true,          /* return user data */
                     false,         /* return EDC ECC */
                     false,         /* return C2 Error information */
                     0,             /* subchannel selection bits */
                     ISO_BLOCKSIZE, /* blocksize*/
                     i_blocks /* Number of blocks. */);
}

/**
   Read sectors using SCSI-MMC GPCMD_READ_CD.
   Can read only up to 25 blocks.
 */
driver_return_code_t read_data_sectors_mmc(void *p_user_data, void *p_buf,
                                           lsn_t i_lsn, uint16_t i_blocksize,
                                           uint32_t i_blocks) {
  const generic_img_private_t *p_env = p_user_data;
  return mmc_read_data_sectors(p_env->cdio, p_buf, i_lsn, i_blocksize,
                               i_blocks);
}

/**
    Set read blocksize (via MMC)
*/
driver_return_code_t set_blocksize_mmc(void *p_user_data,
                                       uint16_t i_blocksize) {
  generic_img_private_t *p_env = p_user_data;
  if (!p_env)
    return DRIVER_OP_UNINIT;
  return mmc_set_blocksize(p_env->cdio, i_blocksize);
}

/** Set the drive speed Set the drive speed in K bytes per second. (via
   MMC).
*/
driver_return_code_t set_speed_mmc(void *p_user_data, int i_speed) {
  generic_img_private_t *p_env = p_user_data;
  if (!p_env)
    return DRIVER_OP_UNINIT;
  return mmc_set_speed(p_env->cdio, i_speed, 0);
}

/**
   Set the drive speed in CD-ROM speed units (via MMC).
*/
driver_return_code_t set_drive_speed_mmc(void *p_user_data, int i_Kbs_speed) {
  generic_img_private_t *p_env = p_user_data;
  if (!p_env)
    return DRIVER_OP_UNINIT;
  return mmc_set_drive_speed(p_env->cdio, i_Kbs_speed);
}

/**
    Get the output port volumes and port selections used on AUDIO PLAY
    commands via a MMC MODE SENSE command using the CD Audio Control
    Page.
*/
driver_return_code_t
mmc_audio_get_volume(CdIo_t *p_cdio, /*out*/ mmc_audio_volume_t *p_volume) {
  uint8_t buf[16];
  int i_rc = mmc_mode_sense(p_cdio, buf, sizeof(buf), CDIO_MMC_AUDIO_CTL_PAGE);

  if (DRIVER_OP_SUCCESS == i_rc) {
    p_volume->port[0].selection = 0xF & buf[8];
    p_volume->port[0].volume = buf[9];
    p_volume->port[1].selection = 0xF & buf[10];
    p_volume->port[1].volume = buf[11];
    p_volume->port[2].selection = 0xF & buf[12];
    p_volume->port[2].volume = buf[13];
    p_volume->port[3].selection = 0xF & buf[14];
    p_volume->port[3].volume = buf[15];
    return DRIVER_OP_SUCCESS;
  }
  return i_rc;
}

/**
  Get the DVD type associated with cd object.
*/
discmode_t mmc_get_dvd_struct_physical_private(void *p_env,
                                               mmc_run_cmd_fn_t run_mmc_cmd,
                                               cdio_dvd_struct_t *s) {
  mmc_cdb_t cdb = {{
      0,
  }};
  unsigned char buf[4 + 4 * 20], *base;
  int i_status;
  uint8_t layer_num = s->physical.layer_num;

  cdio_dvd_layer_t *layer;

  if (!p_env)
    return DRIVER_OP_UNINIT;
  if (!run_mmc_cmd)
    return DRIVER_OP_UNSUPPORTED;

  if (layer_num >= CDIO_DVD_MAX_LAYERS)
    return -EINVAL;

  CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_READ_DVD_STRUCTURE);
  cdb.field[6] = layer_num;
  cdb.field[7] = CDIO_DVD_STRUCT_PHYSICAL;
  cdb.field[9] = sizeof(buf) & 0xff;

  i_status = run_mmc_cmd(p_env, mmc_timeout_ms, mmc_get_cmd_len(cdb.field[0]),
                         &cdb, SCSI_MMC_DATA_READ, sizeof(buf), &buf);
  if (0 != i_status)
    return CDIO_DISC_MODE_ERROR;

  base = &buf[4];
  layer = &s->physical.layer[layer_num];

  /*
   * place the data... really ugly, but at least we won't have to
   * worry about endianness in userspace.
   */
  memset(layer, 0, sizeof(*layer));
  layer->book_version = base[0] & 0xf;
  layer->book_type = base[0] >> 4;
  layer->min_rate = base[1] & 0xf;
  layer->disc_size = base[1] >> 4;
  layer->layer_type = base[2] & 0xf;
  layer->track_path = (base[2] >> 4) & 1;
  layer->nlayers = (base[2] >> 5) & 3;
  layer->track_density = base[3] & 0xf;
  layer->linear_density = base[3] >> 4;
  layer->start_sector = base[5] << 16 | base[6] << 8 | base[7];
  layer->end_sector = base[9] << 16 | base[10] << 8 | base[11];
  layer->end_sector_l0 = base[13] << 16 | base[14] << 8 | base[15];
  layer->bca = base[16] >> 7;

  return (discmode_t)DRIVER_OP_SUCCESS;
}

/**
  Get the media catalog number (MCN) or the ISRC

  Note: string is malloc'd so caller should free() then returned
  string when done with it.

  @param p_cdio
  @param i_track track number
  @param sub_chan_param 2 for MCN, 3 for ISRC

  @return malloc'd string holding the MCN or ISRC on success
          or NULL on failure
 */
char *mmc_get_mcn_isrc_private(const CdIo_t *p_cdio, track_t i_track,
                               unsigned char sub_chan_param) {
  char buf[24]; /* 4 header + 20 data (MMC-4 tables 424, 431, 432) */
  unsigned int num_data;
  size_t length;
  driver_return_code_t i_rc;

  switch (sub_chan_param) {
  case CDIO_SUBCHANNEL_MEDIA_CATALOG: /* MCN */
    length = CDIO_MCN_SIZE;
    break;
  case CDIO_SUBCHANNEL_TRACK_ISRC: /* ISRC */
    length = CDIO_ISRC_SIZE;
    break;
  default:
    return NULL;
  }

  /* inquire number of available reply bytes
     workaround for bad device drivers
   */
  num_data = 4; /* header only */
  i_rc =
      mmc_read_subchannel(p_cdio, i_track, sub_chan_param, &num_data, buf, 0);

  if (i_rc != DRIVER_OP_SUCCESS)
    return NULL;

  if (num_data > sizeof(buf))
    num_data = sizeof(buf);

  if (num_data < 9 + length)
    return NULL; /* Not enough data available */

  i_rc =
      mmc_read_subchannel(p_cdio, i_track, sub_chan_param, &num_data, buf, 0);
  if (i_rc != DRIVER_OP_SUCCESS)
    return NULL;

  if (num_data < 9 + length)
    return NULL; /* Not enough data returned */

  if (!(buf[8] & 0x80)) /* MCVAL / TCVAL bit indicates a valid response */
    return NULL;        /* MCN/ISRC not valid */
  return strndup(&buf[9], length);
}

driver_return_code_t
mmc_set_blocksize_private(void *p_env, const mmc_run_cmd_fn_t run_mmc_cmd,
                          uint16_t i_blocksize) {
  mmc_cdb_t cdb = {{
      0,
  }};

  struct {
    uint8_t reserved1;
    uint8_t medium;
    uint8_t reserved2;
    uint8_t block_desc_length;
    uint8_t density;
    uint8_t number_of_blocks_hi;
    uint8_t number_of_blocks_med;
    uint8_t number_of_blocks_lo;
    uint8_t reserved3;
    uint8_t block_length_hi;
    uint8_t block_length_med;
    uint8_t block_length_lo;
  } mh;

  if (!p_env)
    return DRIVER_OP_UNINIT;
  if (!run_mmc_cmd)
    return DRIVER_OP_UNSUPPORTED;

  memset(&mh, 0, sizeof(mh));
  mh.block_desc_length = 0x08;

  /* while i_blocksize is uint16_t, this expression is always 0 */
  mh.block_length_hi = (i_blocksize >> 16) & 0xff;

  mh.block_length_med = (i_blocksize >> 8) & 0xff;
  mh.block_length_lo = (i_blocksize >> 0) & 0xff;

  CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_MODE_SELECT_6);

  cdb.field[1] = 1 << 4;
  cdb.field[4] = 12;

  return run_mmc_cmd(p_env, mmc_timeout_ms, mmc_get_cmd_len(cdb.field[0]), &cdb,
                     SCSI_MMC_DATA_WRITE, sizeof(mh), &mh);
}

/***********************************************************
  User-accessible Operations.
************************************************************/
/**
  Read Audio Subchannel information

  @param p_cdio the CD object to be acted upon.
*/
driver_return_code_t
mmc_audio_read_subchannel(CdIo_t *p_cdio, cdio_subchannel_t *p_subchannel) {
  mmc_cdb_t cdb;
  driver_return_code_t i_rc;
  cdio_mmc_subchannel_t mmc_subchannel;

  if (!p_cdio)
    return DRIVER_OP_UNINIT;

  memset(&mmc_subchannel, 0, sizeof(mmc_subchannel));
  mmc_subchannel.format = CDIO_CDROM_MSF;
  memset(&cdb, 0, sizeof(mmc_cdb_t));

  CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_READ_SUBCHANNEL);
  CDIO_MMC_SET_LEN16(cdb.field, 7, sizeof(cdio_mmc_subchannel_t));

  cdb.field[1] = CDIO_CDROM_MSF;
  cdb.field[2] = 0x40; /* subq */
  cdb.field[3] = CDIO_SUBCHANNEL_CURRENT_POSITION;
  cdb.field[6] = 0; /* track number (only in isrc mode, ignored) */

  i_rc = mmc_run_cmd(p_cdio, mmc_timeout_ms, &cdb, SCSI_MMC_DATA_READ,
                     sizeof(cdio_mmc_subchannel_t), &mmc_subchannel);
  if (DRIVER_OP_SUCCESS == i_rc) {
    p_subchannel->format = mmc_subchannel.format;
    p_subchannel->audio_status = mmc_subchannel.audio_status;
    p_subchannel->address = mmc_subchannel.address;
    p_subchannel->control = mmc_subchannel.control;
    p_subchannel->track = mmc_subchannel.track;
    p_subchannel->index = mmc_subchannel.index;
    p_subchannel->abs_addr.m = cdio_to_bcd8(mmc_subchannel.abs_addr[1]);
    p_subchannel->abs_addr.s = cdio_to_bcd8(mmc_subchannel.abs_addr[2]);
    p_subchannel->abs_addr.f = cdio_to_bcd8(mmc_subchannel.abs_addr[3]);
    p_subchannel->rel_addr.m = cdio_to_bcd8(mmc_subchannel.rel_addr[1]);
    p_subchannel->rel_addr.s = cdio_to_bcd8(mmc_subchannel.rel_addr[2]);
    p_subchannel->rel_addr.f = cdio_to_bcd8(mmc_subchannel.rel_addr[3]);
  }
  return i_rc;
}

/**
   Get the block size used in read requests, via MMC (e.g. READ_10,
   READ_MSF, ...)

   @param p_cdio the CD object to be acted upon.
   @return the blocksize if > 0; error if <= 0
*/
int mmc_get_blocksize(CdIo_t *p_cdio) {
  int i_status;

  uint8_t buf[255] = {
      0,
  };
  uint8_t *p;

  /* First try using the 6-byte MODE SENSE command. */
  i_status =
      mmc_mode_sense_6(p_cdio, buf, sizeof(buf), CDIO_MMC_R_W_ERROR_PAGE);

  if (DRIVER_OP_SUCCESS == i_status && buf[3] >= 8) {
    p = &buf[4 + 5];
    return CDIO_MMC_GET_LEN16(p);
  }

  /* Next try using the 10-byte MODE SENSE command. */
  i_status =
      mmc_mode_sense_10(p_cdio, buf, sizeof(buf), CDIO_MMC_R_W_ERROR_PAGE);
  p = &buf[6];
  if (DRIVER_OP_SUCCESS == i_status && CDIO_MMC_GET_LEN16(p) >= 8) {
    return CDIO_MMC_GET_LEN16(p);
  }

  return DRIVER_OP_UNSUPPORTED;
}

/**
   Return the number of length in bytes of the Command Descriptor
   buffer (CDB) for a given MMC command. The length will be
   either 6, 10, or 12.
*/
uint8_t mmc_get_cmd_len(uint8_t scsi_cmd) {
  static const uint8_t scsi_cdblen[8] = {6, 10, 10, 12, 12, 12, 10, 10};
  return scsi_cdblen[((scsi_cmd >> 5) & 7)];
}

/**
   Return the size of the CD in logical block address (LBA) units.
   @param p_cdio the CD object to be acted upon.
   @return the lsn. On error 0 or CDIO_INVALD_LSN.
*/
lsn_t mmc_get_disc_last_lsn(const CdIo_t *p_cdio) {
  mmc_cdb_t cdb = {{
      0,
  }};
  uint8_t buf[12] = {
      0,
  };

  lsn_t retval = 0;
  int i_status;

  /* Operation code */
  CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_READ_TOC);

  cdb.field[1] = 0; /* lba; msf: 0x2 */

  /* Format */
  cdb.field[2] = CDIO_MMC_READTOC_FMT_TOC;

  CDIO_MMC_SET_START_TRACK(cdb.field, CDIO_CDROM_LEADOUT_TRACK);

  CDIO_MMC_SET_LEN16(cdb.field, 7, sizeof(buf));

  i_status = mmc_run_cmd(p_cdio, mmc_timeout_ms, &cdb, SCSI_MMC_DATA_READ,
                         sizeof(buf), buf);

  if (i_status)
    return CDIO_INVALID_LSN;

  {
    int i;
    for (i = 8; i < 12; i++) {
      retval <<= 8;
      retval += buf[i];
    }
  }

  return retval;
}

/**
   Return the discmode as reported by the SCSI-MMC Read (FULL) TOC
   command.

   Information was obtained from Section 5.1.13 (Read TOC/PMA/ATIP)
   pages 56-62 from the MMC draft specification, revision 10a
   at http://www.t10.org/ftp/t10/drafts/mmc/mmc-r10a.pdf See
   especially tables 72, 73 and 75.
*/
discmode_t mmc_get_discmode(const CdIo_t *p_cdio)

{
  uint8_t buf[14] = {
      0,
  };
  mmc_cdb_t cdb;

  memset(&cdb, 0, sizeof(mmc_cdb_t));

  CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_READ_TOC);
  CDIO_MMC_SET_LEN16(cdb.field, 7, sizeof(buf));

  cdb.field[1] = CDIO_CDROM_MSF; /* The MMC-5 spec may require this. */
  cdb.field[2] = CDIO_MMC_READTOC_FMT_FULTOC;

  mmc_run_cmd(p_cdio, 2000, &cdb, SCSI_MMC_DATA_READ, sizeof(buf), buf);
  if (buf[7] == 0xA0) {
    if (buf[13] == 0x00) {
      if (buf[5] & 0x04)
        return CDIO_DISC_MODE_CD_DATA;
      else
        return CDIO_DISC_MODE_CD_DA;
    } else if (buf[13] == 0x10)
      return CDIO_DISC_MODE_CD_I;
    else if (buf[13] == 0x20)
      return CDIO_DISC_MODE_CD_XA;
  }
  return CDIO_DISC_MODE_NO_INFO;
}

/**
   Get drive capabilities for a device.
   @param p_cdio the CD object to be acted upon.
   @return the drive capabilities.
*/
void
mmc_get_drive_cap (CdIo_t *p_cdio,
                   /*out*/ cdio_drive_read_cap_t  *p_read_cap,
                   /*out*/ cdio_drive_write_cap_t *p_write_cap,
                   /*out*/ cdio_drive_misc_cap_t  *p_misc_cap)
{
  /* Largest buffer size we use. */
#define BUF_MAX 2048
  uint8_t buf[BUF_MAX+2] = { 0, };

  int i_status;
  uint16_t i_data = BUF_MAX;
  int page = CDIO_MMC_ALL_PAGES;

  if ( ! p_cdio )  return;
 retry:

  /* In the first run we run MODE SENSE 10 we are trying to get the
     length of the data features. */
  i_status = mmc_mode_sense_10(p_cdio, buf, 8, CDIO_MMC_ALL_PAGES);

  if (DRIVER_OP_SUCCESS == i_status) {
    uint16_t i_data_try = (uint16_t) CDIO_MMC_GET_LEN16(buf);
    if (i_data_try < BUF_MAX) i_data = i_data_try;
  }

  /* Now try getting all features with length set above, possibly
     truncated or the default length if we couldn't get the proper
     length. */
  i_status = mmc_mode_sense_10(p_cdio, buf, i_data, CDIO_MMC_ALL_PAGES);
  if (0 != i_status && CDIO_MMC_CAPABILITIES_PAGE != page) {
    page =  CDIO_MMC_CAPABILITIES_PAGE;
    goto retry;
  }

  if (DRIVER_OP_SUCCESS == i_status) {
    uint8_t *p;
    uint8_t *p_max = buf + 256;

    *p_read_cap  = 0;
    *p_write_cap = 0;
    *p_misc_cap  = 0;

    /* set to first sense mask, and then walk through the masks */
    p = buf + 8;
    while( (p < &(buf[2+i_data])) && (p < p_max) )       {
      uint8_t which_page;

      which_page = p[0] & 0x3F;
      switch( which_page )
	{
	case CDIO_MMC_AUDIO_CTL_PAGE:
	case CDIO_MMC_R_W_ERROR_PAGE:
	case CDIO_MMC_CDR_PARMS_PAGE:
	  /* Don't handle these yet. */
	  break;
	case CDIO_MMC_CAPABILITIES_PAGE:
	  mmc_get_drive_cap_buf(p, p_read_cap, p_write_cap, p_misc_cap);
	  break;
	default: ;
	}
      p += (p[1] + 2);
    }
  } else {
    cdio_info("%s: %s\n", "error in MODE_SELECT", strerror(errno));
    *p_read_cap  = CDIO_DRIVE_CAP_ERROR;
    *p_write_cap = CDIO_DRIVE_CAP_ERROR;
    *p_misc_cap  = CDIO_DRIVE_CAP_ERROR;
  }
  return;
}

/* Helper: issue plain SCSI INQUIRY (CDIO_MMC_GPCMD_INQUIRY), EVPD = 0
   (standard). Fills buf up to buf_len and returns 0 on success or negative on
   error.
*/
static int mmc_issue_inquiry(CdIo_t *p_cdio, uint8_t *buf, uint8_t buf_len) {
  mmc_cdb_t cdb = {.field = {
       CDIO_MMC_GPCMD_INQUIRY, 0x00, /* EVPD = 0, standard INQUIRY */
       0x00, /* Page code (unused when EVPD=0) */
       0x00, /* MSB allocation length is 0. */
       0x00, buf_len, /* LSB allocation length */
       0x00  /* Control */
    }};
  int i_status;

  if (!p_cdio)
    return -1;

  CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_INQUIRY);

  i_status = mmc_run_cmd(p_cdio, mmc_timeout_ms, &cdb, SCSI_MMC_DATA_READ,
                         sizeof(buf), &buf);
  return i_status;
}
/**
   Get drive capabilities for a device.
   @param p_cdio the CD object to be acted upon.
   @return the drive capabilities.
*/

/* Size we request from INQUIRY: large enough to include version descriptors.
   Note: this should be less than 256 so the number fits in one byte.
 */
#define MMC_INQUIRY_ALLOC 252

/**
   Get the MMC level supported by the device via INQUIRY and the Version field..

   Parse INQUIRY Version and search for version descriptors indicating
   MMC support. Returns CDIO_INQUIRY_VERSION_* or CDIO_INQUIRY_VERSION_NONE on transport
   failure.
*/
cdio_mmc_inquiry_version_t mmc_get_drive_mmc_cap_from_inquiry_version(CdIo_t *p_cdio)
{
  /* Largest buffer size we use. */
  uint8_t buf[MMC_INQUIRY_ALLOC];
  uint8_t scsi_version;
  int rc = mmc_issue_inquiry(p_cdio, buf, sizeof(buf));
  size_t additional_len = (size_t)buf[4];
  size_t total_len = 5 + additional_len;

  if (rc != 0) {
    /* transport failure */
    return CDIO_INQUIRY_VERSION_NONE;
  }

  /* Basic sanity checks: minimum INQUIRY length is 36 bytes (standard) */
  if (buf[4] < 31) {
    /* additional length field too small to contain version descriptors */
    return CDIO_INQUIRY_VERSION_WEIRD;
  }

  /* The standard INQUIRY response contains the 'additional length' at byte 4.
     The total response length is additional_length + 5. We requested a larger
     buffer already; compute the actual available length from the device's
     reported additional length to avoid reading past the device-returned
     data. */
  if (total_len > sizeof(buf)) {
    total_len = sizeof(buf);
  }

  scsi_version = buf[2] & 0x07;

  switch (scsi_version) {
  case CDIO_INQUIRY_VERSION_1:
  case CDIO_INQUIRY_VERSION_1a:
  case CDIO_INQUIRY_VERSION_2:
  case CDIO_INQUIRY_VERSION_3:
  case CDIO_INQUIRY_VERSION_45:
  case CDIO_INQUIRY_VERSION_5:
    return (cdio_mmc_inquiry_version_t)scsi_version;
  default:
    return CDIO_INQUIRY_VERSION_WEIRD;
  }
}

/**
   Get the MMC level supported by the device.
*/
cdio_mmc_level_t
mmc_get_drive_mmc_cap(CdIo_t *p_cdio)
{
  uint8_t buf[256] = { 0, };
  uint8_t len;
  int rc = mmc_mode_sense(p_cdio, buf, sizeof(buf),
			  CDIO_MMC_CAPABILITIES_PAGE);

  if (DRIVER_OP_SUCCESS != rc) {
    return CDIO_MMC_LEVEL_NONE;
  }

  len = buf[1];
  if (16 > len) {
    return CDIO_MMC_LEVEL_WEIRD;
  } else if (28 <= len) {
    return CDIO_MMC_LEVEL_3;
  } else if (24 <= len) {
    return CDIO_MMC_LEVEL_2;
  } else if (20 <= len) {
    return CDIO_MMC_LEVEL_1;
  } else {
    return CDIO_MMC_LEVEL_WEIRD;
  }
}

/**
   Get the DVD type associated with cd object.

   @param p_cdio the CD object to be acted upon.
   @return the DVD discmode.
*/
discmode_t mmc_get_dvd_struct_physical(const CdIo_t *p_cdio,
                                       cdio_dvd_struct_t *s) {
  if (!p_cdio)
    return -2;
  return mmc_get_dvd_struct_physical_private(p_cdio->env,
                                             p_cdio->op.run_mmc_cmd, s);
}

/**
  Get the CD-ROM hardware info via a MMC INQUIRY command.
  False is returned if we had an error getting the information.

  @param p_cdio the CD object to be acted upon.
  @return true if we were able to get hardware info, false if we had
  an error.
*/
bool mmc_get_hwinfo(const CdIo_t *p_cdio,
                    /*out*/ cdio_hwinfo_t *hw_info) {
  int i_status; /* Result of MMC command */
  char buf[36] = {
      0,
  }; /* Place to hold returned data */
  mmc_cdb_t cdb = {{
      0,
  }}; /* Command Descriptor Block */

  if (!p_cdio || !hw_info)
    return false;

  CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_INQUIRY);
  cdb.field[4] = sizeof(buf);

  i_status = mmc_run_cmd(p_cdio, mmc_timeout_ms, &cdb, SCSI_MMC_DATA_READ,
                         sizeof(buf), &buf);
  if (i_status == 0) {

    memcpy(hw_info->psz_vendor, buf + 8, sizeof(hw_info->psz_vendor) - 1);
    hw_info->psz_vendor[sizeof(hw_info->psz_vendor) - 1] = '\0';
    memcpy(hw_info->psz_model, buf + 8 + CDIO_MMC_HW_VENDOR_LEN,
           sizeof(hw_info->psz_model) - 1);
    hw_info->psz_model[sizeof(hw_info->psz_model) - 1] = '\0';
    memcpy(hw_info->psz_revision,
           buf + 8 + CDIO_MMC_HW_VENDOR_LEN + CDIO_MMC_HW_MODEL_LEN,
           sizeof(hw_info->psz_revision) - 1);
    hw_info->psz_revision[sizeof(hw_info->psz_revision) - 1] = '\0';
    return true;
  }
  return false;
}

/**
  Find out if media has changed since the last call.
  @param p_cdio the CD object to be acted upon.
  @return 1 if media has changed since last call, 0 if not. Error
  return codes are the same as driver_return_code_t
*/
int mmc_get_media_changed(const CdIo_t *p_cdio) {
  uint8_t status_buf[2];
  int i_status;

  i_status = mmc_get_event_status(p_cdio, status_buf);
  if (i_status != DRIVER_OP_SUCCESS)
    return i_status;
  return (status_buf[0] & 0x02) ? 1 : 0;
}

/**
   Get the media catalog number (MCN) from the CD via MMC.

   @param p_cdio the CD object to be acted upon.
   @return the media catalog number r NULL if there is none or we
   don't have the ability to get it.

   Note: The caller must free the returned string with cdio_free()
   when done with it.

*/
char *mmc_get_mcn(const CdIo_t *p_cdio) {
  if (!p_cdio)
    return NULL;
  return mmc_get_mcn_isrc_private(p_cdio, 0, CDIO_SUBCHANNEL_MEDIA_CATALOG);
}

/**
   Get the international standard recording code (ISRC) of the track via MMC.
   @param p_cdio the CD object to be acted upon.
   @param i_track the track to get the ISRC info for.
   @return the international standard recording code or NULL if there is
   none or we don't have the ability to get it.

   Note: The caller must free the returned string with cdio_free()
   when done with it.

*/
char *mmc_get_track_isrc(const CdIo_t *p_cdio, track_t i_track) {
  if (!p_cdio)
    return NULL;
  return mmc_get_mcn_isrc_private(p_cdio, i_track, CDIO_SUBCHANNEL_TRACK_ISRC);
}

/**
  Read cdtext information for a CdIo_t object .

  @return pointer to data on success, NULL on error or CD-Text information
  does not exist.

  Note: the caller must free the returned memory

*/
uint8_t *mmc_read_cdtext(const CdIo_t *p_cdio) {

  unsigned char buf[4];
  unsigned char *wdata;
  int i_status;
  unsigned int i_cdtext;

  if (!p_cdio)
    return NULL;

  /* We may need to give CD-Text a little more time to complete. */
  /* First off, just try and read the size */
  i_cdtext = 4;
  i_status = mmc_read_toc_cdtext(p_cdio, &i_cdtext, buf, 0);

  if (i_status != DRIVER_OP_SUCCESS) {
    return NULL;
  }

  if (i_cdtext > CDTEXT_LEN_BINARY_MAX + 2)
    i_cdtext = CDTEXT_LEN_BINARY_MAX + 4;
  else
    i_cdtext += 2; /* data length does not include the data length field */

  wdata = malloc(i_cdtext); /* is zeroed in mmc_toc_read_cdtext */

  /* Read all of it */
  i_status = mmc_read_toc_cdtext(p_cdio, &i_cdtext, wdata, 0);

  if (i_status != DRIVER_OP_SUCCESS) {
    free(wdata);
    return NULL;
  }

  return wdata;
}

/**
  Find out if media tray is open or closed.
  @param p_cdio the CD object to be acted upon.
  @return 1 if media is open, 0 if closed. Error
  return codes are the same as driver_return_code_t
*/
int mmc_get_tray_status(const CdIo_t *p_cdio) {
  uint8_t status_buf[2];
  int i_status;

  i_status = mmc_get_event_status(p_cdio, status_buf);
  if (i_status != DRIVER_OP_SUCCESS)
    return i_status;
  return (status_buf[1] & 0x01) ? 1 : 0;
}

/* Added in version 0.83 by scdbackup */
/**
   Obtain the SCSI sense reply of the most-recently-performed MMC command.
   These bytes give an indication of possible problems which occurred in
   the drive while the command was performed. With some commands they tell
   about the current state of the drive (e.g. 00h TEST UNIT READY).
   @param p_cdio CD structure set by cdio_open().

   @param sense returns the sense bytes received from the drive.
   This is allocated memory or NULL if no sense bytes are
   available. Dispose non-NULL pointers by cdio_free() when no longer
   needed.  See SPC-3 4.5.3 Fixed format sense data.  SCSI error
   codes as of SPC-3 Annex D, MMC-5 Annex F: sense[2]&15 = Key ,
   sense[12] = ASC , sense[13] = ASCQ

   @return number of valid bytes in sense, 0 in case of no sense
   bytes available, <0 in case of internal error.
  */
int mmc_last_cmd_sense(const CdIo_t *p_cdio,
                       cdio_mmc_request_sense_t **pp_sense) {
  generic_img_private_t *gen;

  if (!p_cdio)
    return DRIVER_OP_UNINIT;
  gen = p_cdio->env;
  *pp_sense = NULL;
  if (gen->scsi_mmc_sense_valid <= 0)
    return 0;
  *pp_sense = calloc(1, gen->scsi_mmc_sense_valid);
  if (*pp_sense == NULL)
    return DRIVER_OP_ERROR;
  memcpy(*pp_sense, gen->scsi_mmc_sense, gen->scsi_mmc_sense_valid);
  return gen->scsi_mmc_sense_valid;
}

/**
  Run a MMC command.

  @param cdio	       CD structure set by cdio_open().
  @param i_timeout     time in milliseconds we will wait for the command
                       to complete. If this value is -1, use the default
                       time-out value.
  @param buf	       Buffer for data, both sending and receiving
  @param len	       Size of buffer
  @param e_direction   direction the transfer is to go
  @param cdb	       CDB bytes. All values that are needed should be set on
                       input. We'll figure out what the right CDB length
                       should be.
*/
driver_return_code_t mmc_run_cmd(const CdIo_t *p_cdio,
                                 unsigned int i_timeout_ms,
                                 const mmc_cdb_t *p_cdb,
                                 cdio_mmc_direction_t e_direction,
                                 unsigned int i_buf,
                                 /*in/out*/ void *p_buf) {
  if (!p_cdio)
    return DRIVER_OP_UNINIT;
  if (!p_cdio->op.run_mmc_cmd)
    return DRIVER_OP_UNSUPPORTED;
  return p_cdio->op.run_mmc_cmd(p_cdio->env, i_timeout_ms,
                                mmc_get_cmd_len(p_cdb->field[0]), p_cdb,
                                e_direction, i_buf, p_buf);
}

/* Added by SukkoPera to allow CDB length to be specified manually */
/**
   Run a Multimedia command (MMC) specifying the CDB length.
   The motivation here is, for example, to use in is an undocumented
   debug command for LG drives (namely E7), whose length is being
   miscalculated by mmc_get_cmd_len(); it doesn't follow the usual
   code number to length conventions. Patch supplied by SukkoPera.

   @param p_cdio        CD structure set by cdio_open().
   @param i_timeout_ms  time in milliseconds we will wait for the command
                        to complete.
   @param p_cdb         CDB bytes. All values that are needed should be set
                         on input.
   @param i_cdb         number of CDB bytes.
   @param e_direction   direction the transfer is to go.
   @param i_buf         Size of buffer
   @param p_buf         Buffer for data, both sending and receiving.

   @return 0 if command completed successfully.
*/

driver_return_code_t mmc_run_cmd_len(const CdIo_t *p_cdio,
                                     unsigned int i_timeout_ms,
                                     const mmc_cdb_t *p_cdb, unsigned int i_cdb,
                                     cdio_mmc_direction_t e_direction,
                                     unsigned int i_buf,
                                     /*in/out*/ void *p_buf) {
  if (!p_cdio)
    return DRIVER_OP_UNINIT;
  if (!p_cdio->op.run_mmc_cmd)
    return DRIVER_OP_UNSUPPORTED;
  return p_cdio->op.run_mmc_cmd(p_cdio->env, i_timeout_ms, i_cdb, p_cdb,
                                e_direction, i_buf, p_buf);
}

/**
  See if CD-ROM has feature with value value
  @return true if we have the feature and false if not.
*/
bool_3way_t mmc_have_interface(CdIo_t *p_cdio,
                               cdio_mmc_feature_interface_t e_interface) {
  int i_status; /* Result of MMC command */
  uint8_t buf[65530] = {
      0,
  }; /* Place to hold returned data */
  mmc_cdb_t cdb = {{
      0,
  }}; /* Command Descriptor Buffer */

  if (!p_cdio || !p_cdio->op.run_mmc_cmd)
    return nope;

  CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_GET_CONFIGURATION);
  CDIO_MMC_SET_LEN16(cdb.field, 7, sizeof(buf));

  cdb.field[1] = CDIO_MMC_GET_CONF_NAMED_FEATURE;
  cdb.field[3] = CDIO_MMC_FEATURE_CORE;

  i_status =
      mmc_run_cmd(p_cdio, 0, &cdb, SCSI_MMC_DATA_READ, sizeof(buf), &buf);
  if (DRIVER_OP_SUCCESS == i_status) {
    uint8_t *p;
    uint32_t i_data;
    uint8_t *p_max = buf + 65530;

    i_data = (unsigned int)CDIO_MMC_GET_LEN32(buf);
    /* set to first sense feature code, and then walk through the masks */
    p = buf + 8;
    while ((p < &(buf[i_data])) && (p < p_max)) {
      uint16_t i_feature;
      uint8_t i_feature_additional = p[3];

      i_feature = CDIO_MMC_GET_LEN16(p);
      if (CDIO_MMC_FEATURE_CORE == i_feature) {
        uint8_t *q = p + 4;
        uint32_t i_interface_standard = CDIO_MMC_GET_LEN32(q);
        if (e_interface == i_interface_standard)
          return yep;
      }
      p += i_feature_additional + 4;
    }
    return nope;
  } else
    return dunno;
}

/**
    Read sectors using SCSI-MMC GPCMD_READ_CD.
    Can read only up to 25 blocks.
*/
driver_return_code_t mmc_read_sectors(const CdIo_t *p_cdio, void *p_buf,
                                      lsn_t i_lsn, int sector_type,
                                      uint32_t i_blocks) {
  mmc_cdb_t cdb = {{
      0,
  }};

  mmc_run_cmd_fn_t run_mmc_cmd;

  if (!p_cdio)
    return DRIVER_OP_UNINIT;
  if (!p_cdio->op.run_mmc_cmd)
    return DRIVER_OP_UNSUPPORTED;

  run_mmc_cmd = p_cdio->op.run_mmc_cmd;

  CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_READ_CD);
  CDIO_MMC_SET_READ_TYPE(cdb.field, sector_type);
  CDIO_MMC_SET_READ_LBA(cdb.field, i_lsn);
  CDIO_MMC_SET_READ_LENGTH24(cdb.field, i_blocks);
  CDIO_MMC_SET_MAIN_CHANNEL_SELECTION_BITS(cdb.field,
                                           CDIO_MMC_MCSB_ALL_HEADERS);

  return run_mmc_cmd(p_cdio->env, mmc_timeout_ms, mmc_get_cmd_len(cdb.field[0]),
                     &cdb, SCSI_MMC_DATA_READ, CDIO_CD_FRAMESIZE_RAW * i_blocks,
                     p_buf);
}

driver_return_code_t mmc_set_blocksize(const CdIo_t *p_cdio,
                                       uint16_t i_blocksize) {
  if (!p_cdio)
    return DRIVER_OP_UNINIT;
  return mmc_set_blocksize_private(p_cdio->env, p_cdio->op.run_mmc_cmd,
                                   i_blocksize);
}

/*
 * Local variables:
 *  c-file-style: "gnu"
 *  tab-width: 8
 *  indent-tabs-mode: nil
 * End:
 */
