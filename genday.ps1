param(
    [Parameter(Mandatory=$true)]
    [Int32]$Day
)

mkdir $Day
cp ./justfile ./$Day
cp ./template.c ./$Day/main.c
cp ./template.c ./$Day/main2.c
