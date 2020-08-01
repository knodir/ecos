# General

This repository contains [Embedded Configurable Operating System](https://en.wikipedia.org/wiki/ECos)(eCos)
related source code by me ([Nodir Kodirov](https://knodir.github.io/)) and
[Doohyun Kim](http://home.konkuk.ac.kr/~doohyun/professor.html).
This work is done as my masters [thesis](./docs/thesis.pdf) in Konkuk University
with the title "Enhancing eCos with EDF Scheduling and Lock-Free Buffer".
In this work, we implement Earliest Deadline First (EDF) scheduler
and Non-Blocking Buffer (NBB) thread communication mechanism in eCos kernel.

# Source code release

We release the source code in two ways. The first one is
GNU General Public License (GPLv2) compliant version that includes complete
source code for easy reproducability.
It is in [source-codes-full](./source-codes-full) folder and it includes following three archives
1) original [ecos-3.0 release](./source-codes-full/ecos-3.0.i386linux.tar.bz2)
from [ecos.sourceware.org](https://ecos.sourceware.org/). It is also directly
accessible from public archives, like [gnu.org](https://gcc.gnu.org/pub/ecos/releases/ecos-3.0/ecos-3.0.i386linux.tar.bz2).
2) the original release modified to implement EDF: [ecos-3.0-edf.tar.bz2](./source-codes-full/ecos-3.0-edf.tar.bz2)
3) the original release modified to implement NBB: [ecos-3.0-nbb.tar.bz2](./source-codes-full/ecos-3.0-nbb.tar.bz2).

We also release our code in easy-to-read way. This is in [my_dev](./my_dev) folder
that only includes our changes to the original [ecos-3.0 release](./source-codes-full/ecos-3.0.i386linux.tar.bz2).
The folder structure is reflective of the structure described in the thesis,
i.e., children of [my_dev](./my_dev) folder are exactly the same as in the thesis work.
More specifically, we include files that are described in
*Figure 4-9: Modified and added EDF specific files* and
*Figure 4-14: NBB development folder structure* in the thesis.
So, if you get the original release, unpack it, and replace its respective files
with the ones in `my_dev` (while respecting folder paths) you will get
[ecos-3.0-edf.tar.bz2](./source-codes-full/ecos-3.0-edf.tar.bz2) and
[ecos-3.0-nbb.tar.bz2](./source-codes-full/ecos-3.0-nbb.tar.bz2).
In other words
==> [original ecos-3.0](./source-codes-full/ecos-3.0.i386linux.tar.bz2) + [my_dev/ecos_dev](./my_dev/ecos_dev) = [ecos-3.0-edf.tar.bz2](./source-codes-full/ecos-3.0-edf.tar.bz2) 
==> [original ecos-3.0](./source-codes-full/ecos-3.0.i386linux.tar.bz2) + [my_dev/ecos_nbb](./my_dev/ecos_nbb) = [ecos-3.0-nbb.tar.bz2](./source-codes-full/ecos-3.0-nbb.tar.bz2).

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
  author={Kodirov, Nodir and Kim, Doohyun},
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

Email me at nodir.qodirov@gmail.com if you have any questions. I'll do my best to help.

