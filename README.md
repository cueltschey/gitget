# GitHelp github cli

## Dependencies
Thus project depends on the following:
* libcurl14
* ncurses

Dependency installation:

Ubuntu/Debian:
> sudo apt-get install libcurl4-openssl-dev libncurses-dev
Fedora:
> sudo dnf install libcurl-devel ncurses-devel
CentOS/RHEL:
> sudo yum install libcurl-devel ncurses-devel
Arch Linux:
> sudo pacman -S curl ncurses
openSUSE:
> sudo zypper install libcurl-devel ncurses-devel

## Installation

To install run the following:
> git clone https://github.com/cueltschey/githelp.git
> cd githelp
> make && sudo make install

Then to authenticate the app generate a token and add it to the environment:
> cat <<EOF > ~/.bashrc 
> # Place your github token here
> export GITHUB_TOKEN=ghp_*********************
> EOF
