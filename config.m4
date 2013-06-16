PHP_ARG_ENABLE(AA,
	[Whether to enable the "Array Analysis" extension],
	[	-enable-aa	Enable "Array Analysis" extension support])

if test $PHP_AA != "no"; then
	PHP_SUBST(AA_SHARED_LIBADD)
	PHP_NEW_EXTENSION(aa, aa.c, $ext_shared)
fi

