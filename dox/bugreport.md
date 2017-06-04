# Bug Reporting   {#bugreport}

## Where to submit bug reports

Bug reports can be submitted to Pulsar-Core's github
issues:

https://github.com/pulsar-chem/Pulsar-Core/issues

## Problems with Building

For issues with building Pulsar, the bug report should include:

  * Version/git commit of Pulsar you are trying to build
  * Compiler output
    * Compile with `make VERBOSE=1` to get more output
  * System information (operating system)
  * Compiler information (GCC vs. Intel, with version numbers)
    * If using Intel, also provide the version of GCC that is being used
      (typically the version in your PATH
  * Versions of any pre-installed dependencies

## Problems with Running

For issues with running Pulsar, include

  * What you were trying to do
  * Input script
    * Please create a minimum-working example (smallest input that triggers the bug)
  * Full output
  * Version/git commit of Pulsar you are trying to build
  * System information (operating system)
  * Versions of any pre-installed dependencies
