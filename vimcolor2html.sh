#!/bin/bash

if [ "$#" -ne 3 ]; then
   echo "vimcolor2html.sh"
   echo "================"
   echo "   Creates colored HTML samples of [EXAMPLE SOURCE] using installed Vim color schemes listed in [COLOR SCHEME LIST]."
   echo "   Writes an HTML file for each color scheme listed in [COLOR SCHEME LIST] TO [OUTPUT FOLDER]."
   echo "Usage"
   echo "====="
   echo "   ./vimcolor2html.sh [COLOR SCHEME LIST] [EXAMPLE SOURCE] [OUTPUT FOLDER]"
   exit 0
fi

EXCLAIM='!'
AR_SCHEME="$(< $1)"
OUTDIR=$3
TESTFILE=$2

#echo $AR_SCHEME
#echo $OUTDIR

mkdir -p "$OUTDIR"

for SCHEME in $AR_SCHEME; do
   OUTFILE="${OUTDIR}/${SCHEME}.html"
   echo "$SCHEME"
   VIMCMD="silent${EXCLAIM} colorscheme ${SCHEME}|TOhtml|w${EXCLAIM} ${OUTFILE}|qa${EXCLAIM}"

   # NO X11 (X), NO SWAP (n), NO VI COMPAT (N)
   vim -XnN -c "$VIMCMD" -- "$TESTFILE"
done
