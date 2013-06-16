#ifndef PHP_AA_H /* BEGIN PHP_AA_H */
#define PHP_AA_H

#define PHP_AA_EXTNAME	"Array Analysis Tool"
#define PHP_AA_EXTVER	"1.0"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_globals.h"

extern zend_module_entry AA_module_entry;
#define phpext_AA_ptr &AA_module_entry

#endif /* END PHP_AA_H */
