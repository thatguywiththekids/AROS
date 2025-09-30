# MetaMake

MetaMake is a version of make which allows you to recursively build targets
in the various directories of a project or even another project.

**Usage:**  
```bash
mmake [<options>] [<metatargets>]
```

To build mmake, just compile `mmake.c`. It doesn't need any other files.

mmake looks for a config file `mmake.config` or `.mmake.config` in the current directory,  
or a file in the environment variable `$MMAKE_CONFIG`,  
or a file `.mmake.config` in the directory `$HOME`.  

This file can contain the following things:

---

### Configuration Options

- **`#`**  
  This must be the first character in a line and begins a comment.  
  Comments are completely ignored by mmake (as are empty lines).

- **`[name]`**  
  This begins a config section for the project `name`.  
  You can build targets for this project by saying `name.target`.

- **`maketool`**  
  Specifies the name of the tool to build a target.  
  Default:  
  ```bash
  make "TOP=$(TOP)" "CURDIR=$(CURDIR)"
  ```

- **`top <dir>`**  
  Specifies the root directory for a project. You will later find this config option in the variable `$(TOP)`.  
  Default: current directory.

- **`defaultmakefilename <filename>`**  
  Specifies the basename for makefiles in your project. Basename means that mmake will consider other files which have this stem and an extension, too.  
  Default: `Makefile`

- **`defaulttarget <target>`**  
  The name of the default target which mmake will try to make if you call it with the name of the project alone.  
  Default: `all`

- **`genmakefilescript <cmdline>`**  
  mmake will check for files with the basename specified in `defaultmakefilename` with the extension `.src`.  
  If such a file is found, the following conditions are checked:  
  - whether this file is newer than the makefile,  
  - whether the makefile doesn't exist,  
  - whether the file `genmakefiledeps` is newer than the makefile.  

  If any of these is true, mmake will call this script with the source file as an extra option, and redirect stdout to `defaultmakefilename`.  
  If missing, mmake will not try to regenerate makefiles.

- **`genmakefiledeps <path>`**  
  File used to decide whether a makefile must be regenerated. Only one such file can be specified.

- **`globalvarfile <path>`**  
  File which contains more variables in the normal `make(1)` syntax.  
  mmake doesn’t know about special things like line continuation. Be careful not to use such variables later (but they won’t break mmake if they exist).

- **`add <path>`**  
  Adds a nonstandard makefile to the list of makefiles for this project.  
  mmake will apply the standard rules to it as if `defaultmakefilename` were this filename.

- **`ignoredir <path>`**  
  Will tell mmake to ignore directories with this name.  
  Example: `ignore CVS` if you use CVS.

- **Other options**  
  Any option not recognized will be added to the list of known variables.  
  Example: `foo bar` will create a variable `$(foo)` that expands to `bar`.

---

### Example Configuration

```ini
# This is a comment
# Options before the first [name] are defaults. Use them for global defaults
defaultoption value

# Special options for the project name. You can build targets for this
# project with "mmake name.target"
[AROS]
# The root dir of the project. The default is the current directory
top /home/digulla/AROS
# Default name for Makefiles
defaultmakefilename makefile
# Default target when running "mmake AROS"
defaulttarget AROS
# Regenerate makefiles if missing/newer
genmakefilescript gawk -f $(TOP)/scripts/genmf.gawk --assign "TOP=$(TOP)"
genmakefiledeps $(TOP)/scripts/genmf.gawk
# Read variables from this file
globalvarfile $(TOP)/config/host.cfg

# Some makefiles must have different names:
#add compiler/include/makefile
#add makefile
```

---

### Metatargets

A metatarget looks like `project.target`.  
Example: `AROS.setup`.

- If nothing is specified, mmake will make the default target of the first project in the config file.  
- If only the project is specified (no target), mmake will make the default target of that project.

---

### How mmake Works

mmake will:

1. Look for all makefiles (regenerating them if necessary).  
2. Check which makefiles provide the metatargets from the command line.  
3. Resolve dependencies between metatargets.  
4. Successively make all required metatargets.

mmake calculates and passes the following variables to `make`:

- `$(TOP)` — path to the top of the project  
- `$(CURDIR)` — path to the current directory relative to `$(TOP)`  
- `$(TARGET)` — the current target (not passed to `make` automatically)

If you need the target in the makefile, use the `maketool` option to modify how it’s invoked.
