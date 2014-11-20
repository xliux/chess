# .bash_profile

export BASH_PROFILE_ALREADY_SOURCED=1

# Get the aliases and functions
if [ -f ~/.bashrc ]; then
  . ~/.bashrc
fi

# User specific environment and startup programs
export PATH=$HOME/bin:$PATH:/sw/bin:$HOME/homebrew/bin

# unix nav
alias ..='cd ..'
alias ...='cd ../..'
alias la='ls -al'
alias pd="pushd"
alias po="popd"

export EDITOR=vim
export LS_COLORS="${LS_COLORS}:di=01;33"
export GNUTERM='x11'

# safety guards
alias rm="rm -i"
alias mv="mv -i"
alias cp="cp -i"

export CHOST=`hostname|cut -d"." -f1`;

shopt -s checkwinsize
set -o vi
