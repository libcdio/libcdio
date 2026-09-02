/*
  Copyright (C) 2013 Rocky Bernstein
  <rocky@gnu.org>

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

/* Tests reading ISO 9660 info from an ISO 9660 image.  */

#ifndef DATA_DIR
#define DATA_DIR "./data"
#endif
/* Set up a CD-DA image to test on which is in the libcdio distribution. */
#define ISO9660_IMAGE_PATH DATA_DIR "/"
#define ISO9660_IMAGE    ISO9660_IMAGE_PATH "copying.iso"
#define ISO9660_IMAGE_RR ISO9660_IMAGE_PATH "copying-rr.iso"

#define SKIP_TEST_RC 77

#ifdef HAVE_CONFIG_H
#include "config.h"
#define __CDIO_CONFIG_H__ 1
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include <cdio/cdio.h>
#include <cdio/iso9660.h>

static void
put_733(uint8_t *p, uint32_t value)
{
  p[0] = value;
  p[1] = value >> 8;
  p[2] = value >> 16;
  p[3] = value >> 24;
  p[4] = value >> 24;
  p[5] = value >> 16;
  p[6] = value >> 8;
  p[7] = value;
}

static unsigned
add_cycle_record(uint8_t *dir, unsigned offset, char name, uint32_t extent,
                 int child_link)
{
  const unsigned length = child_link ? 46 : 34;
  uint8_t *record = dir + offset;

  memset(record, 0, length);
  record[0] = length;
  put_733(record + 2, extent);
  put_733(record + 10, ISO_BLOCKSIZE);
  record[25] = child_link ? 0 : ISO_DIRECTORY;
  record[32] = 1;
  record[33] = name;
  if (child_link) {
    record[34] = 'C';
    record[35] = 'L';
    record[36] = 12;
    record[37] = 1;
    put_733(record + 38, 888);
  }
  return offset + length;
}

static int
check_rock_ridge_cycle(void)
{
  uint8_t pvd[ISO_BLOCKSIZE] = {0};
  uint8_t dir[ISO_BLOCKSIZE] = {0};
  unsigned offset = 0;
  iso9660_t *p_iso = NULL;
  CdioISO9660FileList_t *entries = NULL;
  FILE *fp = NULL;
  int ret = 1;
#ifdef HAVE_MKSTEMP
  char psz_tmp[] = "libcdio-rock-cycle-XXXXXX";
  int fd = mkstemp(psz_tmp);
  if (fd < 0)
    return 1;
  fp = fdopen(fd, "wb");
#else
  char *psz_tmp = tmpnam(NULL);
  if (psz_tmp)
    fp = fopen(psz_tmp, "wb");
#endif

  if (!fp)
    goto out;
  if (fseek(fp, 21 * ISO_BLOCKSIZE - 1, SEEK_SET) != 0
      || fputc(0, fp) == EOF)
    goto out;

  pvd[0] = 1;
  memcpy(&pvd[1], "CD001", 5);
  pvd[6] = 1;
  put_733(&pvd[80], 21);
  pvd[128] = 0;
  pvd[129] = 8;
  pvd[156] = 34;
  put_733(&pvd[158], 20);
  put_733(&pvd[166], ISO_BLOCKSIZE);
  pvd[181] = 2;
  pvd[188] = 1;

  offset = add_cycle_record(dir, offset, '\0', 20, 0);
  offset = add_cycle_record(dir, offset, '\1', 20, 0);
  offset = add_cycle_record(dir, offset, 'A', 20, 0);
  add_cycle_record(dir, offset, 'X', 999, 1);

  if (fseek(fp, 16 * ISO_BLOCKSIZE, SEEK_SET) != 0
      || fwrite(pvd, 1, sizeof(pvd), fp) != sizeof(pvd))
    goto out;
  memset(pvd, 0, sizeof(pvd));
  pvd[0] = 255;
  memcpy(&pvd[1], "CD001", 5);
  pvd[6] = 1;
  if (fwrite(pvd, 1, sizeof(pvd), fp) != sizeof(pvd)
      || fseek(fp, 20 * ISO_BLOCKSIZE, SEEK_SET) != 0
      || fwrite(dir, 1, sizeof(dir), fp) != sizeof(dir)
      || fclose(fp) != 0)
    goto out;
  fp = NULL;

  p_iso = iso9660_open_ext(psz_tmp, ISO_EXTENSION_ALL);
  if (!p_iso)
    goto out;
  entries = iso9660_ifs_readdir(p_iso, "/");
  if (!entries)
    goto out;
  iso9660_filelist_free(entries);
  entries = NULL;
  ret = 0;

out:
  if (entries)
    iso9660_filelist_free(entries);
  if (p_iso)
    iso9660_close(p_iso);
  if (fp)
    fclose(fp);
  remove(psz_tmp);
  return ret;
}

static unsigned
add_directory_record(uint8_t *dir, unsigned offset, uint32_t extent,
                     uint8_t flags, uint8_t name)
{
  uint8_t *record = dir + offset;

  memset(record, 0, 34);
  record[0] = 34;
  put_733(record + 2, extent);
  put_733(record + 10, ISO_BLOCKSIZE);
  record[25] = flags;
  record[28] = 1;
  record[32] = 1;
  record[33] = name;
  return offset + 34;
}

static int
check_excessive_path_depth(void)
{
  const unsigned depth = 300;
  uint8_t pvd[ISO_BLOCKSIZE] = {0};
  uint8_t dir[ISO_BLOCKSIZE] = {0};
  char path[depth * 2];
  unsigned i;
  unsigned offset;
  iso9660_t *p_iso = NULL;
  iso9660_stat_t *p_stat = NULL;
  FILE *fp = NULL;
  int ret = 1;
#ifdef HAVE_MKSTEMP
  char psz_tmp[] = "libcdio-iso-depth-XXXXXX";
  int fd = mkstemp(psz_tmp);
  if (fd < 0)
    return 1;
  fp = fdopen(fd, "wb");
#else
  char *psz_tmp = tmpnam(NULL);
  if (psz_tmp)
    fp = fopen(psz_tmp, "wb");
#endif

  if (!fp)
    goto out;
  if (fseek(fp, (22 + depth) * ISO_BLOCKSIZE - 1, SEEK_SET) != 0
      || fputc(0, fp) == EOF)
    goto out;

  pvd[0] = 1;
  memcpy(&pvd[1], "CD001", 5);
  pvd[6] = 1;
  put_733(&pvd[80], 22 + depth);
  pvd[128] = 0;
  pvd[129] = 8;
  pvd[156] = 34;
  put_733(&pvd[158], 20);
  put_733(&pvd[166], ISO_BLOCKSIZE);
  pvd[181] = ISO_DIRECTORY;
  pvd[188] = 1;
  pvd[189] = 0;

  if (fseek(fp, 16 * ISO_BLOCKSIZE, SEEK_SET) != 0
      || fwrite(pvd, 1, sizeof(pvd), fp) != sizeof(pvd))
    goto out;
  memset(pvd, 0, sizeof(pvd));
  pvd[0] = 255;
  memcpy(&pvd[1], "CD001", 5);
  pvd[6] = 1;
  if (fwrite(pvd, 1, sizeof(pvd), fp) != sizeof(pvd))
    goto out;

  for (i = 0; i <= depth; i++) {
    offset = 0;
    memset(dir, 0, sizeof(dir));
    offset = add_directory_record(dir, offset, 20 + i, ISO_DIRECTORY, 0);
    offset = add_directory_record(dir, offset, i ? 19 + i : 20,
                                  ISO_DIRECTORY, 1);
    add_directory_record(dir, offset, 21 + i, ISO_DIRECTORY, 'D');
    if (fseek(fp, (20 + i) * ISO_BLOCKSIZE, SEEK_SET) != 0
        || fwrite(dir, 1, sizeof(dir), fp) != sizeof(dir))
      goto out;
  }
  if (fclose(fp) != 0)
    goto out;
  fp = NULL;

  for (i = 0; i < depth; i++) {
    path[2 * i] = 'D';
    path[2 * i + 1] = '/';
  }
  path[2 * depth - 1] = '\0';

  p_iso = iso9660_open_ext(psz_tmp, ISO_EXTENSION_NONE);
  if (!p_iso)
    goto out;
  p_stat = iso9660_ifs_stat(p_iso, path);
  if (p_stat)
    iso9660_stat_free(p_stat);
  else
    ret = 0;

out:
  if (p_iso)
    iso9660_close(p_iso);
  if (fp)
    fclose(fp);
  remove(psz_tmp);
  return ret;
}

static int
check_malformed_record(void)
{
  const char *source_name = DATA_DIR "/bad-dir.iso";
  char temp_name[] = "libcdio-malformed-rr-XXXXXX";
  FILE *source = NULL;
  FILE *temp = NULL;
  iso9660_t *p_iso = NULL;
  uint8_t buffer[4096];
  size_t nread;
  int fd;
  int result = 1;

  source = fopen(source_name, "rb");
  fd = mkstemp(temp_name);
  if (!source || fd < 0) {
    if (fd >= 0)
      close(fd);
    goto out;
  }
  temp = fdopen(fd, "wb+");
  if (!temp) {
    close(fd);
    goto out;
  }

  while ((nread = fread(buffer, 1, sizeof(buffer), source)) != 0) {
    if (fwrite(buffer, 1, nread, temp) != nread)
      goto out;
  }
  if (fseek(temp, 23 * ISO_BLOCKSIZE, SEEK_SET) != 0
      || fputc(1, temp) == EOF)
    goto out;
  if (fclose(temp) != 0) {
    temp = NULL;
    goto out;
  }
  temp = NULL;

  p_iso = iso9660_open_ext(temp_name, ISO_EXTENSION_ROCK_RIDGE);
  if (!p_iso || iso9660_have_rr(p_iso, 0) != dunno)
    goto out;
  result = 0;

out:
  if (p_iso)
    iso9660_close(p_iso);
  if (temp)
    fclose(temp);
  if (source)
    fclose(source);
  remove(temp_name);
  return result;
}

int
main(int argc, const char *argv[])
{
  char const *psz_fname;
  iso9660_t *p_iso;

  psz_fname = ISO9660_IMAGE;
  p_iso = iso9660_open_ext(psz_fname, ISO_EXTENSION_ROCK_RIDGE);

  if (NULL == p_iso) {
    fprintf(stderr, "Sorry, couldn't open %s as an ISO-9660 image\n",
	    psz_fname);
    return 1;
  }

  if (iso9660_have_rr(p_iso, 0) != nope) {
    fprintf(stderr, "-- Should not find Rock Ridge for %s\n", psz_fname);
    return 2;
  } else {
    printf("-- Good! Did not find Rock Ridge in %s\n", psz_fname);
  }

  iso9660_close(p_iso);

  psz_fname = ISO9660_IMAGE_RR;
  p_iso = iso9660_open_ext(psz_fname, ISO_EXTENSION_ROCK_RIDGE);

  if (NULL == p_iso) {
    fprintf(stderr, "Sorry, couldn't open %s as an ISO-9660 image\n",
	    psz_fname);
    return 3;
  }

  if (iso9660_have_rr(p_iso, 0) == yep) {
      printf("-- Good! found Rock Ridge in %s\n", psz_fname);
  } else {
    fprintf(stderr, "-- Should have found Rock Ridge for %s\n", psz_fname);
    return 2;
  }

  if (iso9660_have_rr(p_iso, 3) == yep) {
      printf("-- Good! Found Rock Ridge again in %s\n", psz_fname);
  } else {
    fprintf(stderr, "-- Should have found Rock Ridge for %s\n", psz_fname);
    return 3;
  }

  iso9660_close(p_iso);

  if (check_rock_ridge_cycle() != 0) {
    fprintf(stderr, "Rock Ridge deep-directory cycle was not rejected\n");
    return 4;
  }

  if (check_excessive_path_depth() != 0) {
    fprintf(stderr, "Excessive ISO path depth was not rejected\n");
    return 5;
  }

  if (check_malformed_record() != 0) {
    fprintf(stderr, "Malformed directory record was not rejected\n");
    return 6;
  }

  return 0;
}
