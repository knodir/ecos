This repository contains Embedded Configurable Operating System (eCos)
related source code from Nodir Kodirov's masters thesis project.
It mainly includes files that implement Earliest Deadline First (EDF) scheduler
and Non-Blocking Buffer (NBB) thread communication mechanism in eCos kernel.

The folder structure is reflective of the structure described in the thesis.
More specifically, we include files that are described in 
<Figure 4-9> Modified and added EDF specific files and
<Figure 4-14> NBB development folder structure.
The hope is that you should be able to reproduce the thesis work with these files.
If this does not happen, feel free to email me at nodir.qodirov@gmail and
I'll do my best to help you out.

# Citations

If you use the thesis work or this repo, please cite them as follows. 

Bibtex entry for thesis work:
```
@mastersthesis{kodirov2010thesis,
  title={Enhancing eCos with EDF Scheduling and Lock-Free Buffer},
  author={Kodirov, Nodir},
  year={2010},
  school={Konkuk University},
  note={Online at knodir.github.io/files/papers/graduate-thesis.pdf}
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
