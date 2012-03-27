#/bin/sh
rm -f classification*
for pname in $(cat $1)
do
  runcppcuter -p classify -D $pname
done
