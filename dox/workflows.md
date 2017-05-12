Workflows                                                           {#workflows}
=========

From a computational chemistry user's standpoint one of the coolest things
Pulsar enables is a uniform workflow via its Python API.  Particularly in the
sciences Python is taking off as a scripting language and there is a wide
variety of packages for performing data analysis.  One of the nicer of these
packages is Jupyter notebooks.  These are interactive Python sessions, where the
results are embedded in the notebook.  For the most part when these notebooks
are used in computational chemistry it is for data processing and not data
collection.  This splinters the workflow.  This page aims to discuss how Pulsar
enables uniform workflows via Jupyter notebooks and to provide notes/tips/*etc.*
on getting the most from this arrangement.

History
-------

At this point in time quite a few electronic structure packages and molecular
mechanics packages have Python APIs.  Consequentially, it is tempting to try
to directly incorporate them into a Jupyter notebook.  However, as researchers
soon discover doing so is prime for loosing hard-earned data.  This is because
for most of these packages their only source of of back-up is the output files.
If a restart is needed one is then forced to resort to parsing the output files
or rerunning the computation.  Typically this is the cause for separating the
computation and the analysis steps.

As should be clear from other parts of this manual, Pulsar includes a cacheing
feature.  This cacheing feature allows a "dump" of the state of the computation
at anytime.  Thus so long as a cache is available (and modules use it correctly)
the user need not fear data loss.  Consequentially, restarting a Jupyter
notebook is perfectly doable and there is no longer a need to treat data
collection any differently than data processing.  The result is a more uniform
workflow that can actually be used in higher-level workflows with littel effort.

Running Basic Pulsar-Based Computations in a Jupyter Notebook
-------------------------------------------------------------

The Pulsar Python API should be usable from a Jupyter notebook in exactly the
same way it would be from a Python script.  The exception to this is the fact
that unless you restart the kernel reexecuting a cell will rerun its contents.
When the contents are a computation this is fine as Pulsar's cache will return
the answer immediatly.  If however the contents are something like adding a
key to the module manager, then an error will be thrown because that key is
already in the module manager.  For the time being we recommend putting setup
(*i.e.* loading modules, changing options, *etc.*) and computation (*i.e.* the
actual energy method calls) in seperate cells that way if you need to rerun a
computation with a tweaked input you can safely rerun the computation cell.

TODO: Add examples and maybe discuss cacheing


HPC and HTC with Jupyter Notebooks
----------------------------------

For running local smaller computations, those that can be run on the current
computer, the considerations in the previous section suffice.  For running
more intensive computations one needs access to more powerful computers, which
typically are only accessible via a network.  Jupyter notebooks can still be
used as a front-end.

TODO: write-up when I figure this out...
