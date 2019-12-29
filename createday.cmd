SET DAY=0%1
SET DAY=%DAY:~-2%
echo %DAY%
mkdir Day%DAY%

curl --output Day%DAY%\Day%DAY%.txt https://adventofcode.com/2019/day/%1/input --cookie "session=%AoCcookie%"