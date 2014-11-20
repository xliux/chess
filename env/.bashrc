if [ "$BASH_PROFILE_ALREADY_SOURCED" == "" ]; then
  source $HOME/.bash_profile
fi
if [ -f $HOME/.bash/bashrc ]; then
  source $HOME/.bash/bashrc
fi
if [ -f $HOME/.xmodmaprc ]; then
  source $HOME/.xmodmaprc
fi
set -o vi
export HOMEBREW_GITHUB_API_TOKEN=109d1b6b1c384573b69e872d593162e9cf44e9ff
