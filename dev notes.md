dev notes.md
------------

# Directory structure

## Content of the Osdk/__final__ folder

- Currently, this folder contains the built version of the OSDK and is stored on version control.
  This causes several concerns:
  1 - The version currently store on the SVN repository is actually not functional.
      Setting the environment variable OSDK to point to it result in a broken build setup
      which fails to compile valid programs (cf http://forum.defence-force.org/viewtopic.php?f=3&t=1847&p=17778#p17785).
  2 - Each platform will produce executables in that folder which will be incompatible of that
      of other platforms, and thus cannot be stored in the source control repository or they
      would overwrite executables of previously compiled platforms.

  Q: How should we handle this?
  1 - Simply remove Osdk/_final_ from source control repositories and add proper ignore rules
      for most used source control systems (svn, git, p4).
  2 - Add a platform specific level such as __final__/<platform>/ and place built results there?
      => Simple and easy to standardize, would allow to use the same build scripts and to keep
      publishing the binaries on source control.
      => Probably a bad idea though.
  -- Decide that only one platform (Windows?) has the right to be submitted into __final__.
    => But this is risky, accidental commits can always happen, so solution 1 is probably better.

# Building the OSDK

## On Windows

### With Visual Studio

- Win32 seems to be the only platform.

  Q: Should we adapt the compiler to also support Win64 (in addition to Win32)?
  Advantages:
  - macOS will soon ditch 32 bit support so we will need to support 64 bit.
  - When Windows ditches 32 bit support we will not have to rush.
    => Maybe this can be done as part of macOS support, then migrated back to Linux/Windows?
  - Should run even faster (more registers, instruction set more optimized).

### With Cygwin

- When building the compiler with make, from its own folder, it is generated in the compiler/ folder.
  Whereas on Windows, it is generated in OSDK/Build/compiler/Debug/.

  Q: Should we uniformize this across platforms?
  => Probably yes, because executables are not meant to be used in their project folder.
     Moreover, if we want to run test scripts, we want them to expect executables to be in the same
     places for all platforms.
     Having different places also confuses revision control systems and might cause them to be
     accidentally submitted.

- Changing execute permissions using chmod, causes a "NULL SID" entry to appear in the corresponding
  file's permissions in the Explorer.

  => Investigate why this is happening and how to fix it and prevent it.

- When cloning the OSDK Git repository, the permissions all have a "NULL SID" entry in the Explorer.

  => Cf above, this is likely due to some Cygwin quirk.
  => Fix and prevent from happening.

# Compiler

- The compiler does not respond to being launched with "--help" or "-h" or no parameters.

  Q: What is the expected behavior when passing no parameters at all?
  => Investigate.
  Q: Is there a way to have it list its usage parameters?
  Q: If not, should we add it a command switch to list usage parameters when "-h", "--help", "/?" on Windows, or nothing is passed?

# Building Oric programs

## On Windows, using bin/make.bat

- If compiler/preprocessor/etc. are missing, the build fails with <some-to-be-determined> error message.

  => I am currently fixing that one by providing a proper error message.

- If compiler/preprocessor/etc. do not have execution permissions, the build fails with an "Access is denied" error message.

  => To properly detect and report.

- When bin/make.bat fails to build (for valid reasons), it should probably set
  the error level so the error is detected by launching tools or build systems.

