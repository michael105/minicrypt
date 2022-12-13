### minicrypt

###### ------------
<br>
Cryptographic hashes:
blake3, blake2s, sha512, md6.

Base encodings:
base91, base32

Ascii table dumper: ascii

###### ------------


Statically linked binaries, linux 64bit, sizes: between 1kB and 5kB: [./bin](./bin) <br> 
Part of ["minicore"](https://github.com/michael105/minicore) - Aiming for a unix system with 64kB.

Since the whole bin directory is less than 20kB, I made a tarball, containing this readme,
the binaries, and accompanying licenses. [minicrypt.tgz](./minicrypt.tgz).
err. going to.

The executables just read from stdin, and write to stdout.

Possible arguments are printed with "-h" (if any).

The hashes are based on the reference implementations,
slightly modified to optimize for size.
(stripped loop unrolls, etc)

Replaced several 32bit instruction with 64bit,
and inserted bswap/ror/rol instructions, where possible.

amd64 only. (Intel and AMD x64)

The full sources, which do not depend on any library, also not libc,
are in the folders, and named blake3sum.src.c, ...

These are generated with minimake, part of [minilib](https://github.com/michael105/minilib). (Another project of mine)



The sources here are quite untidy. My fault.
On the other hand, the disassembly of the binaries should be readable.
If you're in doubt, I stripped them heavily to save space, even the section headers are stripped.
Therefore you'd need something like ndisasm to get the disassembly. The entry function
is somewhere around 0xc2 (pop rdi).


I hand the cleanup of the sources to the compiler, with best wishes.

###### ------------


The following licenses apply to the unmodified implementations.


* md6: MIT
* blake3: CC0, or alternatively Apache License 2.0
* sha512: BSD 2-clause
* base91: BSD 3-clause

base32, ascii entered by me.

For my work in this repository, BSD 3-clause does apply.
(sources and binaries)<br>
minilib comes with other License terms. (BSD like, with attribution),
but I don't apply those terms to the binaries or sources here.


###### ------------

References:

 * https://github.com/BLAKE3-team/BLAKE3
 * https://base91.sourceforge.net


###### ------------
<br>


somehow - I guess, I'm going to enlarge the 64kB core to 128 kB.
Or I'll need to have a multicall binary. 
I don't like that, I don't like bloating software either, I'm screwed

Maybe, I just shouldn't count "nonstandard" tools - md6, blake3!?


misc, December 2022.<br>
https://github.com/michael105



---
<pre>
BSD 3-Clause License

Copyright (c) 2022, Michael (misc) Myer (https://github.com/michael105)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
</pre>

