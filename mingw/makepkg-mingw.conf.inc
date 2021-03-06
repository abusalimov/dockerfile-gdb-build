PREFIX=${PREFIX-/win}

_makepkg_mingw_conf_name="${BASH_SOURCE}"

source $(dirname ${_makepkg_mingw_conf_name})/../makepkg.conf.inc

BUILDENV+=(ccache)

# Override strip to use the tool from cross-toolchain
tidy_strip() {
	if check_option "strip" "y"; then
		msg2 "$(gettext "Stripping unneeded symbols from binaries and libraries...") [OVERRIDDEN in ${_makepkg_mingw_conf_name}]"
		# make sure library stripping variables are defined to prevent excess stripping
		[[ -z ${STRIP_SHARED+x} ]] && STRIP_SHARED="-S"
		[[ -z ${STRIP_STATIC+x} ]] && STRIP_STATIC="-S"

		# if check_option "debug" "y"; then

		# 	dbgdir="$pkgdirbase/$pkgbase-@DEBUGSUFFIX@/usr/lib/debug"
		# 	dbgsrc="$pkgdirbase/$pkgbase-@DEBUGSUFFIX@/usr/src/debug"
		# 	mkdir -p "$dbgdir" "$dbgsrc"
		# fi

		local binary strip_flags
		find . -type f -perm -u+w -print0 2>/dev/null | while read -rd '' binary ; do
			case "$(file -bi "$binary")" in
				*application/x-dosexec*)  # Libraries (.dll) and Binaries (.exe)
					strip_flags="$STRIP_SHARED";;
				*application/x-archive*)    # Libraries (.a)
					strip_flags="$STRIP_STATIC";;
				# *application/x-object*)
				# 	case "$binary" in
				# 		*.ko)                   # Kernel module
				# 			strip_flags="$STRIP_SHARED";;
				# 		*)
				# 			continue;;
				# 	esac;;
				# *application/x-executable*) # Binaries
				# 	strip_flags="$STRIP_BINARIES";;
				*)
					continue ;;
			esac
			${CHOST}-strip ${strip_flags} "$binary"
		done
	fi
}

# vim: set ft=sh ts=2 sw=2 et: