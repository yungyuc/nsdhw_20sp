======================
Homework assignment #4
======================

To turn in your homework, you need **a PR** and **an E3 entry for the PR URL**.
Duration: 4th May (Mon) -- 20th May (Wed).  Cut-off date is 27th May (Wed).

Question
========

Based on what you wrote in the previous homework assignment or the course
notes, use tiling technique to speed up the matrix-matrix multiplication
without limiting the matrix or tiling size.  The speed-up must be more than 20%
(runtime must be less than 80%).

Submission
==========

Your PR needs to be created against the ``hw4`` branch.  (Not ``master``!) You
need to create a sub-directory using exactly your GitHub username as your
working directory (``nsdhw_20sp/hw4/$username/``).  The hierarchy should be::

  - nsdhw_20sp (<- repository root)
    - hw4
      - username (<- your working directory)
        - Makefile
        - xxx.{cc|cpp|cxx}
        - (any other files you need)

In **every commit** in your PR, you can only change the files in your working
directory.  You may not touch anything else.  Failure to follow may cost you
points.

Please make sure your PR is compatible with the latest NSD AMI on AWS (it uses
Ubuntu 18.04 LTS) in ``us-east-1``.  You are not required to use the AMI for
doing the homework, but the grader uses it to run your code.  If your code
fails to build or run on it, **you can lose points**.

There are totally 5 points in this homework assignment.  The grader will run on
a `m5.large` in `us-east-1` and check the result from the ``validate.py``
script.  The benchmark result may go off on different hardware.  Please test
your code in the same or a similar environment.  The grader may attempt to run
the script with the environment variable ``PRELOAD_MKL`` set, if
``validate.py`` doesn't pass without it.  Your work is considered pass in
either case.

CI is set up to provide additional guidance.  The grader may reference the CI
result but will not use it to solely determine your grade.  To get it running,
your branch name should start with ``<username>-hw4**``, e.g,
``yungyuc-hw4-attempt1``.  See the CI configuration file and the Github Action
documentation for more information.

.. vim: set ft=rst ff=unix fenc=utf8 et sw=2 ts=2 sts=2:
