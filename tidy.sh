bugprone_checks='bugprone-*'
cppcore_checks='cppcoreguidelines-*'
modernize_checks='modernize-*,-modernize-use-trailing-return-type'
performance_checks='performance-*,-performance-no-int-to-ptr'
readability_checks='readability-*,-readability-magic-numbers'
portability_checks='portability-*'
static_checks='clang-analyzer-*'

checks="-*,$bugprone_checks,$cppcore_checks,$modernize_checks,$performance_checks,$readability_checks,$portability_checks,$static_checks"
# checks="-*,$readability_checks"

run-clang-tidy -checks="$checks" -header-filter='.*include/project.*' -fix '.*/src.*\.cpp'
