if [[ $# != 3 ]]; then
    echo error : expected 3 arguments found $#
    exit
fi
testname=$1
correct=$2
input=$3

awk ' NR==FNR { a[NR]=$0; next } $0 != a[FNR] { print FNR " : " $0 " <-> " a[FNR]; }' $testname $correct > comparedDiff

awk -F' ' '{
    print $1;
}' comparedDiff > .temp_lines

while IFS= read -r line
do
  let "line2=$line*2"
  let "line3=$line2+1"
  printf "$line2 : "
  head -n $line2 $input | tail -n 1;
  printf "$line3 : "
  head -n $line3 $input | tail -n 1;
  echo
done < .temp_lines


rm -f .temp_lines