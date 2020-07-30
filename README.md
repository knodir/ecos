# General

This repository contains [Embedded Configurable Operating System](https://en.wikipedia.org/wiki/ECos)(eCos)
related source code from [my](https://knodir.github.io/) masters thesis project.
It mainly includes files that implement Earliest Deadline First (EDF) scheduler
and Non-Blocking Buffer (NBB) thread communication mechanism in eCos kernel.

The folder structure is reflective of the structure described in the thesis.
More specifically, we include files that are described in 
*Figure 4-9: Modified and added EDF specific files* and
*Figure 4-14: NBB development folder structure* of the thesis work.
The hope is that you should be able to reproduce the work with these files.
If this does not happen, feel free to email me at nodir.qodirov@gmail and
I'll do my best to help you out.

# Documentation

The main reference should be the [thesis](./docs/thesis.pdf) work.
You can also refer to the higher level [presenation](./docs/ecos-edf-implementation.ppt)
of the source code that I used to describe my implementation in our lab seminars.
Lastly, there is another [slide deck](./docs/ecos-seminar-2011.ppt) that I used to present this work
after my graduation.

# Citations

If you use the thesis work or this repo, please cite them as follows. 

Bibtex entry for thesis work:
```
@mastersthesis{kodirov2010thesis,
  title={Enhancing eCos with EDF Scheduling and Lock-Free Buffer},
  author={Kodirov, Nodir},
  year={2010},
  school={Konkuk University},
  note={Online at https://github.com/knodir/ecos/docs/thesis.pdf}
}
```

Bibtex entry for the Github repository:
```
@unpublished{kodirov2010repo,
  title={Source code for Enhancing eCos with EDF Scheduling and Lock-Free Buffer},
  author={Kodirov, Nodir},
  year={2020},
  note={Online at https://github.com/knodir/ecos}
}
```

# Known issues

The `kapidata.hxx` file described in Figure 4-14: NBB development folder structure
should be called `kapidata.h`. The thesis has a typo. In other words,
```
- ecos_nbb_rep/ecos-3.0/packages/kernel/v3_0/include/kapidata.hxx
+ ecos_nbb_rep/ecos-3.0/packages/kernel/v3_0/include/kapidata.h
```
