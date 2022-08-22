# tao - (tiny automatic opener)
Tao is a very basic xdg-open replacement written in about 100 lines of C.
## Installation
Tao's only dependency is libmagic.
After installing libmagic, run the following commands:
```
git clone https://github.com/wooosh/tao
cd tao
make # Compiles tao
make config # Copies the config file to $XDG_CONFIG_HOME, and if unset, ~/.config
sudo make install # Copies the tao binary to /usr/local/bin
```
To remove tao, run the following commands:
```
rm ~/.config/tao.conf
sudo make uninstall
```
## Configuration
Instead of being configured with `.desktop` files like xdg-open, tao is configured with a single config file placed in `~/.config/tao.conf`. The configuration file format is very close to ini, but has some key differences:
- No sections needed
- Comments are indicated with # at the beginning of a line
- Comments cannot be placed after a rule

The current format of these rules is `mode:param = program`.
```
# Filename endswith is ends:string
ends:.html = code
# Filename startswith is strt: string
strt:LICENSE = license-pick
# Mime types are represented with mime:mimetype
mime:text/x-c = vim
```
## Planned Features
- [ ] `printf` style substution of filenames
- [ ] Change mime type matching to be startswith
- [ ] Add optional regex support
- [ ] Add dry run mode
- [ ] Add web address support
