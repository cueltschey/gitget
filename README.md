# GitHelp github cli

## Dependencies
Thus project depends on the following:
* libcurl14
* ncurses

**Dependency installation:**

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
export GITHUB_USERNAME=your_username
export GITHUB_TOKEN=ghp_*********************
EOF

To verify the installation and authentication run githelp with -v
> githelp -v

## Usage

Once GitHelp is installed and authenticated, it can be used to create and clone repos more easily

**Flags:**
* `githelp -p` : this flag lists a users repos and clones the selected repo.
* `githelp -c` : this flag creates a repo with a given name, either private or public.
* `githelp -a` : this flag clones any repo with the name and owner of the repo.



