cd tests
yeslist=`ls | grep yes`
nolist=`ls | grep no`

echo "======================== TEST BEGIN ========================"

for file in $yeslist
do 
{
    ../build/solver -in $file > /dev/null
    errorCode=$?
    if [ $errorCode == 0 ]
    then
        echo "Test: $file Pass!"
    else
        echo "Test: $file Failed!"
    fi
}
done

for file in $nolist
do
{
    ../build/solver -in $file > /dev/null
    errorCode=$?
    if [ $errorCode == 1 ]
    then
        echo "Test: $file Pass!"
    else
        echo "Test: $file Failed!"
    fi
}
done
wait
echo "======================== TEST END ========================"