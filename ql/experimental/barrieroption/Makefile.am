
AM_CPPFLAGS = -I${top_builddir} -I${top_srcdir}

this_includedir=${includedir}/${subdir}
this_include_HEADERS = \
    all.hpp \
    analyticdoublebarrierbinaryengine.hpp \
	analyticdoublebarrierengine.hpp \
	mcdoublebarrierengine.hpp \
    binomialdoublebarrierengine.hpp \
    discretizeddoublebarrieroption.hpp \
    doublebarrieroption.hpp \
    doublebarriertype.hpp \
    perturbativebarrieroptionengine.hpp \
    quantodoublebarrieroption.hpp \
    suowangdoublebarrierengine.hpp \
    vannavolgabarrierengine.hpp \
    vannavolgadoublebarrierengine.hpp \
    vannavolgainterpolation.hpp

cpp_files = \
    analyticdoublebarrierbinaryengine.cpp \
	analyticdoublebarrierengine.cpp \
	mcdoublebarrierengine.cpp \
    discretizeddoublebarrieroption.cpp \
    doublebarrieroption.cpp \
    doublebarriertype.cpp \
    perturbativebarrieroptionengine.cpp \
    quantodoublebarrieroption.cpp \
    suowangdoublebarrierengine.cpp \
    vannavolgabarrierengine.cpp

if UNITY_BUILD

nodist_libBarrierOption_la_SOURCES = unity.cpp

unity.cpp: Makefile.am
	echo "/* This file is automatically generated; do not edit.     */" > $@
	echo "/* Add the files to be included into Makefile.am instead. */" >> $@
	echo >> $@
	for i in $(cpp_files); do \
		echo "#include \"${subdir}/$$i\"" >> $@; \
	done

EXTRA_DIST = $(cpp_files)

else

libBarrierOption_la_SOURCES = $(cpp_files)

endif

noinst_LTLIBRARIES = libBarrierOption.la

all.hpp: Makefile.am
	echo "/* This file is automatically generated; do not edit.     */" > ${srcdir}/$@
	echo "/* Add the files to be included into Makefile.am instead. */" >> ${srcdir}/$@
	echo >> ${srcdir}/$@
	for i in $(filter-out all.hpp, $(this_include_HEADERS)); do \
		echo "#include <${subdir}/$$i>" >> ${srcdir}/$@; \
	done
	echo >> ${srcdir}/$@
	subdirs='$(SUBDIRS)'; for i in $$subdirs; do \
		echo "#include <${subdir}/$$i/all.hpp>" >> ${srcdir}/$@; \
	done

