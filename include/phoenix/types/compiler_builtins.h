/*
 * Copyright © 2024 Guillot Tony <tony.guillot@protonmail.com>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _COMPILER_BUILTINS_H
#define _COMPILER_BUILTINS_H

#ifdef __cplusplus
extern "C" {
#endif

#define LIKELY(x)   __builtin_expect((x), 1)
#define UNLIKELY(x) __builtin_expect((x), 0)
#define ASSUME(x)   __builtin_assume(x)

#if defined(__has_c_attribute) && __STDC_VERSION__ >= 202000L

#if __has_c_attribute(clang::diagnose_as_builtin)
#define DIAGNOSE_AS_BUILTIN(...) [[clang::diagnose_as_builtin(__VA_ARGS__)]]
#else
#define DIAGNOSE_AS_BUILTIN(...)
#endif

#if __has_c_attribute(gnu::aligned)
#define ALIGNED(x) [[gnu::aligned(x)]]
#else
#define ALIGNED(x) __attribute__((aligned(x)))
#endif

#if __has_c_attribute(gnu::section)
#define SECTION(x) [[gnu::section(x)]]
#else
#define SECTION(x) __attribute__((section(x), used))
#endif

#if __has_c_attribute(gnu::zero_call_used_regs)
#define ZERO_CALL(x) [[gnu::zero_call_used_regs(x)]]
#else
#define ZERO_CALL(x) __attribute__((zero_call_used_regs(x)))
#endif

#if __has_c_attribute(gnu::deprecated)
#define DEPRECATED(m) [[gnu::deprecated(m)]]
#else
#define DEPRECATED(m) __attribute__((deprecated(m)))
#endif

#if __has_c_attribute(clang::enum_extensibility)
#define ENUM_EXTENSIBILITY(type) [[clang::enum_extensibility(type)]]
#else
#define ENUM_EXTENSIBILITY(type) __attribute__((enum_extensibility(type)))
#endif

#if __has_c_attribute(clang::flag_enum)
#define FLAG_ENUM [[clang::flag_enum]]
#else
#define FLAG_ENUM __attribute__((flag_enum))
#endif

#if __has_c_attribute(gnu::flatten)
#define FLATTEN [[gnu::flatten]]
#else
#define FLATTEN __attribute__((flatten))
#endif

#if __has_c_attribute(gnu::always_inline)
#define ALWAYS_INLINE [[gnu::always_inline]]
#else
#define ALWAYS_INLINE __attribute__((always_inline))
#endif

#if __has_c_attribute(clang::noinline)
#define NO_INLINE [[clang::noinline]]
#else
#define NO_INLINE __attribute__((noinline))
#endif

#if __has_c_attribute(nodiscard)
#define NODISCARD [[nodiscard]]
#else
#define NODISCARD __attribute__((warn_unused_result))
#endif

#if __has_c_attribute(gnu::const)
#define CONST [[gnu::const]]
#else
#define CONST __attribute__((const))
#endif

#if __has_c_attribute(gnu::patchable_function_entry)
#define PAD_FUNCTION [[gnu::patchable_function_entry(6, 0)]]
#else
#define PAD_FUNCTION __attribute__((patchable_function_entry(6, 0)))
#endif

#if __has_c_attribute(gnu::used)
#define USED [[gnu::used]]
#else
#define USED __attribute__((used))
#endif

#if __has_c_attribute(maybe_unused)
#define MAYBE_UNUSED [[maybe_unused]]
#else
#define MAYBE_UNUSED __attribute__((unused))
#endif

#if __has_c_attribute(gnu::packed)
#define PACKED [[gnu::packed]]
#else
#define PACKED __attribute__((packed))
#endif

#if __has_c_attribute(noreturn)
#define NORETURN [[noreturn]]
#else
#define NORETURN __attribute__((noreturn))
#endif

#if __has_c_attribute(clang::noderef)
#define NODEREF [[clang::noderef]]
#else
#define NODEREF __attribute__((noderef))
#endif

#if __has_c_attribute(gnu::cleanup)
#define CLEANUP [[gnu::cleanup]]
#else
#define CLEANUP __attribute__((cleanup))
#endif

#if __has_c_attribute(clang::called_once)
#define CALLED_ONCE [[clang::called_once]]
#else
#define CALLED_ONCE __attribute__((called_once))
#endif

#if __has_c_attribute(clang::noescape)
#define NOESCAPE [[clang::noescape]]
#else
#define NOESCAPE __attribute__((noescape))
#endif

#if __has_c_attribute(gnu::nonnull)
#define NONNULL [[gnu::nonnull]]
#else
#define NONNULL __attribute__((nonnull))
#endif

#if __has_c_attribute(gnu::returns_nonnull)
#define RETURNS_NONNULL [[gnu::returns_nonnull]]
#else
#define RETURNS_NONNULL __attribute__((returns_nonnull))
#endif

#if __has_c_attribute(fallthrough)
#define FALLTHROUGH [[fallthrough]]
#else
#define FALLTHROUGH __attribute__((fallthrough))
#endif

#if __has_c_attribute(gnu::transparent_union)
#define TRANSPARENT_UNION [[gnu::transparent_union]]
#else
#define TRANSPARENT_UNION __attribute__((transparent_union))
#endif

#if __has_c_attribute(gnu::randomize_layout)
#define RANDOMIZE_LAYOUT [[gnu::randomize_layout]]
#else
#define RANDOMIZE_LAYOUT __attribute__((randomize_layout))
#endif

#else

#define DIAGNOSE_AS_BUILTIN(...)
#define ALIGNED(x)               __attribute__((aligned(x)))
#define SECTION(x)               __attribute__((section(x), used))
#define ZERO_CALL(x)             __attribute__((zero_call_used_regs(x)))
#define DEPRECATED(m)            __attribute__((deprecated(m)))
#define ENUM_EXTENSIBILITY(type) __attribute__((enum_extensibility(type)))
#define FLAG_ENUM                __attribute__((flag_enum))
#define FLATTEN                  __attribute__((flatten))
#define ALWAYS_INLINE            __attribute__((always_inline))
#define NO_INLINE                __attribute__((noinline))
#define NODISCARD                __attribute__((warn_unused_result))
#define CONST                    __attribute__((const))
#define PAD_FUNCTION             __attribute__((patchable_function_entry(6, 0)))
#define USED                     __attribute__((used))
#define MAYBE_UNUSED             __attribute__((unused))
#define PACKED                   __attribute__((packed))
#define NORETURN                 __attribute__((noreturn))
#define NODEREF                  __attribute__((noderef))
#define CLEANUP                  __attribute__((cleanup))
#define CALLED_ONCE              __attribute__((called_once))
#define NOESCAPE                 __attribute__((noescape))
#define NONNULL                  __attribute__((nonnull))
#define RETURNS_NONNULL          __attribute__((returns_nonnull))
#define FALLTHROUGH              __attribute__((fallthrough))
#define TRANSPARENT_UNION        __attribute__((transparent_union))
#define RANDOMIZE_LAYOUT         __attribute__((randomize_layout))

#endif

#endif /* _COMPILER_BUILTINS_H */
