echo "Running tests..."
echo

#chmod +x gendeck.o

input1=$1
input2=$2
input3=$3
input4=$4
input5=$5

expected_output=$(cat "$input5")

output=$(./main.o "$input1" "$input2" "$input3" "$input4")

#echo deckprint2.txt
#hexdump -C deckprint2.txt
echo
echo
echo
#echo ./main.o "$input1" "$input2" "$input3"
#./main.o "$input1" "$input2" "$input3" | hexdump -C

if [ $? -eq 0 ] ; then
  echo "Pass: Program exited zero"
else
  echo "Fail: Program did not exit zero"
  exit 1
fi

if [ "$output" == "$expected_output" ] ; then
  echo "Pass: Output is correct"
else
  echo "Expected '$expected_output' but got: '$output'"
  exit 1
fi

echo
echo "All tests passed."

exit 0
