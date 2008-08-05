#! /bin/sh

##  bootstrap file for the VLC media player
##
## Copyright (C) 2005-2008 the VideoLAN team
##
##  Authors: Sam Hocevar <sam@zoy.org>
##           Rémi Denis-Courmont <rem # videolan # org>

if test "$#" != "0"; then
  echo "Usage: $0"
  echo "  Calls autoreconf to generate m4 macros and prepare Makefiles."
  exit 1
fi

set -e
set -x

##
## Check for various tools
##

# Check for gettext
if gettextize --version >/dev/null 2>&1; then
# Autopoint is available from 0.11.3, but we need 0.11.5
if expr `gettextize --version | sed -e '1s/[^0-9]*//' -e q` \
        '>=' 0.11.5 >/dev/null 2>&1; then
  # We have gettext, and a recent version! Everything is cool.
  autopoint=autopoint
  GETTEXT=yes
else
  # User's gettext is too old. try to continue anyway.
  autopoint=:
  GETTEXT=old
fi;else
  set +x
  echo "you need gettextize (package gettext-devel or gettext)"
  exit 1
fi

# Check for pkg-config
if pkg-config --version >/dev/null 2>&1; then
  # We have pkg-config, everything is cool.
  PKGCONFIG=yes
else
  PKGCONFIG=no
fi

###
###  classic bootstrap stuff
###
set -x

autoreconf --install --force --verbose ${ACLOCAL_ARGS}

# Shut up
set +x

##
##  Tell the user about gettext, pkg-config
##
if [ "${GETTEXT}" = "old" ]; then
  cat << EOF

==========================================================
NOTE: you have an old version of gettext installed on your
system. The build will work, but if your system does not have
libintl you will not have internationalization support.
We suggest upgrading to gettext 0.11.5 or later.
EOF
fi

if [ "$PKGCONFIG" = "no" ]; then
  cat << EOF

==============================================================
NOTE: "pkg-config" is missing from your system. Certain
libraries may not be detected properly.
EOF
fi

echo "Successfully bootstrapped"
