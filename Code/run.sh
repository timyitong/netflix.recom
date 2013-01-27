cd Implementation
arglist=""
for path in "$@"
do
   arglist="$arglist $path"
done
make --silent
./recommend $arglist
