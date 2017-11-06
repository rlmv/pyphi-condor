dnl aclocal.m4 generated automatically by aclocal 1.4

dnl Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

dnl Starting here is a bunch of local macros



AC_DEFUN(AC_CXX_DYNAMIC_CAST,
        [AC_CACHE_CHECK(whether the compiler supports dynamic_cast<>,
        ac_cv_cxx_dynamic_cast,
        [AC_LANG_SAVE
         AC_LANG_CPLUSPLUS
         AC_TRY_COMPILE([#include <typeinfo>
        class Base { public : Base () {} virtual void f () = 0;};
        class Derived : public Base { public : Derived () {} virtual void f () {} };],[
        Derived d; Base& b=d; return dynamic_cast<Derived*>(&b) ? 0 : 1;],
         ac_cv_cxx_dynamic_cast=yes, ac_cv_cxx_dynamic_cast=no)
         AC_LANG_RESTORE
        ])
        if test "$ac_cv_cxx_dynamic_cast" = yes; then
          AC_DEFINE(HAVE_DYNAMIC_CAST,,[define if the compiler supports dynamic_cast<>])
        fi
        ])


