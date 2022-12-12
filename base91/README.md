#### base91


original author and work: please see README


I modified the translation table, so the output of base91 can be used
within shell scripts or C source code, without having to escape
these special characters: "'\

So all characters between (and including) 0x21 (space) and 0x7e (~)
are used, excluding the 3 chars with special meanings.

If you'd have the need to translate between this base91 encoding,
and the original one, insert <code>| tr '-"' '"-' |</code> into the pipe.








