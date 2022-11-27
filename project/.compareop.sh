if [[ $# != 3 ]]; then
    if [[ ($# == 1) && ($1 == -h) ]]; then
      echo  Help : This may be used to compare two input files, 
      echo  Help : the one to be tested and the correct one. It 
      echo  Help : prints the difference as well as the input 
      echo  Help : that led to mismatch.
      echo
      echo  Usage:
      echo  $0 \`output1\` \`output2\` \`input\`
      echo
      echo  Help : \`output1\` and \`output2\` are the files
      echo  Help : that will be compared. \`input\` should be 
      echo  Help : the corresponding input file.
      exit
    fi
    echo error : expected 3 arguments found $#
    echo Run \`$0 -h\` for help
    exit
fi
output1=$1
output2=$2
input=$3

awk ' NR==FNR { a[NR]=$0; next } $0 != a[FNR] { print FNR " : " $0 " <-> " a[FNR]; }' $output1 $output2 > comparedDiff

awk -F' ' '{
    print $1;
}' comparedDiff > .temp_lines

numLines=$(wc -l < .temp_lines)
echo $numLines mismatch found
echo 

ctr=0
while IFS= read -r line
do
  ctr=$((ctr+1))
  let "line2=$line*2"
  let "line3=$line2+1"
  printf "$line2 : "
  head -n $line2 $input | tail -n 1;
  printf "$line3 : "
  head -n $line3 $input | tail -n 1;
  printf "Difference : "
  head -n $ctr comparedDiff | tail -n 1;
  echo
  if [[ $ctr == 11 ]]; then
    echo More than 10 wrong cases. Truncating...
    break
  fi
done < .temp_lines

rm -f .temp_lines
