[![Packaging status](https://repology.org/badge/tiny-repos/libcdio.svg)](https://repology.org/project/libcdio/versions) [![latest packaged version(s)](https://repology.org/badge/latest-versions/libcdio.svg)](https://repology.org/project/libcdio/versions)


# Introduction

_Note: See [README-libcdio.md](README-libcdio.md) for installation instructions._


The libcdio package contains a library for CD-ROM and CD image access. Applications wishing to be oblivious of the OS- and
device-dependent properties of a CD-ROM or of the specific details of various CD-image formats may benefit from using this library.

A library for working with ISO-9660 filesystems libiso9660 is
included. A generic interface for issuing MMC (multimedia commands) is
also part of the libcdio library.

Also included is a library for working with ISO-9660 filesystems.

The CD-DA error/jitter correction library from
[cdparanoia](http://www.xiph.org/paranoia) is included as a separate
library licenced under GPL v2.

Some support for disk image types like CDRWin's BIN/CUE format,
cdrdao's TOC format, and Nero's NRG format are available. Therefore,
applications that use this library also have the ability to read disc
images as though they were CD's.

The library is written in C, however there are OO C++, Perl, Python
and Ruby wrappers to interface to the library. However C++ is the only
one that is bundled with this package, and the interfaces provide only
a subset of the full features of the library.

Also included in the libcdio package are a number of utility programs:

* `cd-info`  - displays CD information: number of tracks, CD-format and
      if possible basic information about the format.  If [libcddb](http://libcddb.sourceforge.net)
	  is available, the `cd-info` program will display CDDB matches on CD-DA discs.
	  And if a new enough version of `libvcdinfo` is available (from the
      vcdimager project), then `cd-info` shows basic VCD information.

* `cd-read`  - performs low-level block reading of a CD or CD image,

* `iso-info` - displays ISO-9660 information from an ISO-9660 image,

* `iso-read` - extracting files from an ISO-9660 image, a version of the
             CD-DA extraction tool cdparanoia which corrects for
             CD-ROM jitter, and a simple curses-based CD player,
             cdda-player using the analog CD-ROM output.

* `cd-paranoia` - port of cdparanoia (CD-DA jitter and error correction)
                using libcdio back-end CD-reading.

There is very limited low-level support for MMC commands on some
platforms. Using MMC writing can be done. However there is currently
little higher level-support for writing. Other libraries like `libburn`,
`libdi`, `libscg`, or `libdvdread` may be helpful.

Some of the projects using libcdio are the Video CD authoring and
ripping tools [VCDImager](http://vcdimager.org), a navigation-capable
Video CD plugin and CD-DA plugins for the media players
[xine](http://xinehq.de), videolan's [vlc](http://videolan.org), media
players [mplayerxp](http://mplayerxp.sourceforge.net/) and
[gmerlin](http://gmerlin.sourceforge.net),
[kiso](http://kiso.sourceforge.net), a KDE GUI for creating,
extracting and editing ISO-9600 images and a Samba vfs module that
allows exporting a CD without mounting it
(<http://ontologistics.net/OpenSource/Samba/index.php>).
