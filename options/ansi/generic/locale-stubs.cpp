
#include <limits.h>
#include <locale.h>
#include <string.h>

#include <bits/ensure.h>
#include <bits/feature.h>

#include <mlibc/debug.hpp>
#include <frg/optional.hpp>

namespace {
	// Values of the C locale are defined by the C standard.
	lconv c_locale_lconv = {
		const_cast<char *>("."), // decimal_point
		const_cast<char *>(""), // thousands_sep
		const_cast<char *>(""), // grouping
		const_cast<char *>(""), // mon_decimal_point
		const_cast<char *>(""), // mon_thousands_sep
		const_cast<char *>(""), // mon_grouping
		const_cast<char *>(""), // positive_sign
		const_cast<char *>(""), // negative_sign
		const_cast<char *>(""), // currency_symbol
		CHAR_MAX, // frac_digits
		CHAR_MAX, // p_cs_precedes
		CHAR_MAX, // n_cs_precedes
		CHAR_MAX, // p_sep_by_space
		CHAR_MAX, // n_sep_by_space
		CHAR_MAX, // p_sign_posn
		CHAR_MAX, // n_sign_posn
		const_cast<char *>(""), // int_curr_symbol
		CHAR_MAX, // int_frac_digits
		CHAR_MAX, // int_p_cs_precedes
		CHAR_MAX, // int_n_cs_precedes
		CHAR_MAX, // int_p_sep_by_space
		CHAR_MAX, // int_n_sep_by_space
		CHAR_MAX, // int_p_sign_posn
		CHAR_MAX // int_n_sign_posn
	};
}

struct __Mlibc_LocaleDesc {
	// identifier of this locale. used in setlocale()
	const char *locale;
};

frg::optional<__Mlibc_LocaleDesc> __mlibc_queryLocale(const char *locale) {
	// our default C locale is the C locale
	if(!strlen(locale))
		return __mlibc_queryLocale("C");

	// for now we only support the C locale
	if(strcmp(locale, "C")) {
		mlibc::infoLogger() << "mlibc: Locale " << locale << " is not supported" << frg::endlog;
		return frg::null_opt;
	}

	__Mlibc_LocaleDesc desc;
	desc.locale = locale;
	return desc;
}

__Mlibc_LocaleDesc __mlibc_collateDesc;
__Mlibc_LocaleDesc __mlibc_ctypeDesc;
__Mlibc_LocaleDesc __mlibc_monetaryDesc;
__Mlibc_LocaleDesc __mlibc_numericDesc;
__Mlibc_LocaleDesc __mlibc_timeDesc;

void __mlibc_initLocale() {
	__mlibc_collateDesc = *__mlibc_queryLocale("C");
	__mlibc_ctypeDesc = *__mlibc_queryLocale("C");
	__mlibc_monetaryDesc = *__mlibc_queryLocale("C");
	__mlibc_numericDesc = *__mlibc_queryLocale("C");
	__mlibc_timeDesc = *__mlibc_queryLocale("C");
}

__Mlibc_LocaleDesc &__mlibc_currentLocale(int category) {
	switch(category) {
	case LC_COLLATE: return __mlibc_collateDesc;
	case LC_CTYPE: return __mlibc_ctypeDesc;
	case LC_MONETARY: return __mlibc_monetaryDesc;
	case LC_NUMERIC: return __mlibc_numericDesc;
	case LC_TIME: return __mlibc_timeDesc;
	default:
		__ensure(!"__mlibc_currentLocale called with bad category");
		__builtin_unreachable();
	}
}

char *setlocale(int category, const char *locale) {
	if(!(category == LC_ALL || category == LC_COLLATE || category == LC_CTYPE
			|| category == LC_MONETARY || category == LC_NUMERIC || category == LC_TIME)) {
		mlibc::infoLogger() << "mlibc: Unexpected value " << category
				<< " for category in setlocale()" << frg::endlog;
		return nullptr;
	}

	if(category == LC_ALL) {
		// ´TODO: implement correct return value when categories differ
		__Mlibc_LocaleDesc current_desc = __mlibc_collateDesc;
		__ensure(!strcmp(current_desc.locale, __mlibc_ctypeDesc.locale));
		__ensure(!strcmp(current_desc.locale, __mlibc_monetaryDesc.locale));
		__ensure(!strcmp(current_desc.locale, __mlibc_numericDesc.locale));
		__ensure(!strcmp(current_desc.locale, __mlibc_timeDesc.locale));
		if(!locale)
			return const_cast<char *>(current_desc.locale);

		frg::optional<__Mlibc_LocaleDesc> new_desc = __mlibc_queryLocale(locale);
		if(!new_desc)
			return nullptr;
		__mlibc_collateDesc = *new_desc;
		__mlibc_ctypeDesc = *new_desc;
		__mlibc_monetaryDesc = *new_desc;
		__mlibc_numericDesc = *new_desc;
		__mlibc_timeDesc = *new_desc;
		return const_cast<char *>(current_desc.locale);
	}

	__Mlibc_LocaleDesc current_desc = __mlibc_currentLocale(category);
	if(!locale)
		return const_cast<char *>(current_desc.locale);

	frg::optional<__Mlibc_LocaleDesc> new_desc = __mlibc_queryLocale(locale);
	if(!new_desc)
		return nullptr;
	__mlibc_currentLocale(category) = *new_desc;
	return const_cast<char *>(current_desc.locale);
}

struct lconv *localeconv(void) {
	return &c_locale_lconv;
}
