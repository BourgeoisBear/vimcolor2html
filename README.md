vimcolor2html.sh
================

## Why?
Live-toggling colorschemes in Vim is slow and error-prone.  This script will take a list of installed Vim color schemes and generate a list of HTML example files from it.

## Usage
```
vimcolor2html.sh
================
   Creates colored HTML samples of [EXAMPLE SOURCE] using installed Vim color schemes listed in [COLOR SCHEME LIST].
   Writes an HTML file for each color scheme listed in [COLOR SCHEME LIST] TO [OUTPUT FOLDER].
Usage
=====
   ./vimcolor2html.sh [COLOR SCHEME LIST] [EXAMPLE SOURCE] [OUTPUT FOLDER]
```

## FYI
* List installed colorschemes in Vim: `:colorscheme <Ctrl-d>`
* Requires `bash` and `vim`
* Thanks to: [Robert Elder's article on using Vim inside a pipe](http://blog.robertelder.org/use-vim-inside-a-unix-pipe-like-sed-or-awk/)
* This will run faster on smaller example files
* Terminal needs to have 256-color support.  If you use `tmux`, start it as `tmux -2`.

## Alternate Homepage
* https://eggplant.pro/vimcolor2html.md

## Resources
* https://github.com/flazz/vim-colorschemes
* https://github.com/rafi/awesome-vim-colorschemes
